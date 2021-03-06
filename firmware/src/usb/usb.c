#include "usb.h"

u8 USB_configuration_number = 0;
bool USB_remote_wakeup_enabled = false;

/*
	The recommended default idle rate (rate when the device is initialized) is 500
	milliseconds for keyboards (delay before first repeat rate) and infinity for
	joysticks and mice.
		- USB HID 1.11 Specification, p. 53
*/
u16 USB_idle_timeout_duration = 500;
u16 USB_idle_timeout_remaining = 0;

void USB_init() {
	/*
		Power On the USB interface
			- Power-On USB pads regulator
			- Configure PLL interface
			- Enable PLL
			- Check PLL lock
			- Enable USB interface
			- Configure USB interface (USB speed, Endpoints configuration...)
			- Wait for USB VBUS information connection
			- Attach USB device

			- ATmega32U4 datasheet, section 21.12 USB Software Operating Modes, p. 266
	*/

	UHWCON = _BV(UVREGE);
	USBCON = _BV(USBE) | _BV(FRZCLK);
	PLLFRQ = _BV(PLLUSB) | _BV(PDIV3) | _BV(PDIV1);
	USB_enable_pll();
	USBCON = (USBCON & ~_BV(FRZCLK)) | _BV(OTGPADE) | _BV(VBUSTE);
	UDCON = 0; // Attach, enable full speed mode
	UDIEN = _BV(SUSPE) | _BV(EORSTE);

	USB_configure_endpoint(USB_ENDPOINT_CONTROL,
		USB_CFG0X_ENDPOINT_TYPE_CONTROL | USB_CFG0X_ENDPOINT_DIRECTION_OUT,
		USB_CFG1X_ENDPOINT_SIZE(USB_ENDPOINT_CONTROL_SIZE));
}

bool _is_idle_timed_out() {
	return USB_idle_timeout_duration && !USB_idle_timeout_remaining;
}

void USB_update() {
	switch (USB_DEVICE_STATE) {
		case USB_DeviceState_Configured:
			if (USB_is_keyboard_report_dirty() || _is_idle_timed_out()) {
				void *report;
				u16 reportSize;
				USB_get_keyboard_report(&report, &reportSize);

				USB_idle_timeout_remaining = USB_idle_timeout_duration;

				USB_select_endpoint(USB_ENDPOINT_KEYBOARD_IN);
				USB_transfer_data(report, reportSize, false, USB_ENDPOINT_KEYBOARD_SIZE);
			}

			if (USB_is_system_report_dirty() || _is_idle_timed_out()) {
				USB_OtherReport_t report;
				USB_get_system_report(&report);

				USB_idle_timeout_remaining = USB_idle_timeout_duration;

				USB_select_endpoint(USB_ENDPOINT_OTHER_IN);
				USB_transfer_data(&report, sizeof(report), false, USB_ENDPOINT_OTHER_SIZE);
			}

			if (USB_is_consumer_report_dirty() || _is_idle_timed_out()) {
				USB_OtherReport_t report;
				USB_get_consumer_report(&report);

				USB_idle_timeout_remaining = USB_idle_timeout_duration;

				USB_select_endpoint(USB_ENDPOINT_OTHER_IN);
				USB_transfer_data(&report, sizeof(report), false, USB_ENDPOINT_OTHER_SIZE);
			}

			break;

		case USB_DeviceState_Suspended:
			if (USB_remote_wakeup_enabled && USB_is_keyboard_report_dirty()) {
				USB_send_remote_wakeup();
			}
			break;
	}
}

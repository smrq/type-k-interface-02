#include "usb.h"

u8 USB_configuration_number = 0;
bool USB_remote_wakeup_enabled = false;
bool USB_using_report_protocol = true;

/*
	The recommended default idle rate (rate when the device is initialized) is 500
	milliseconds for keyboards (delay before first repeat rate) and infinity for
	joysticks and mice.
		- USB HID 1.11 Specification, p. 53
*/
u16 USB_idle_timeout_duration = 500;
u16 USB_idle_timeout_remaining = 0;

local void _HID_send_report() {
	union {
		USB_BootKeyboardReport_t boot;
		USB_NkroKeyboardReport_t nkro;
	} report = { 0 };
	u8 reportSize;
	bool shouldSend;

	if (USB_using_report_protocol) {
		shouldSend = USB_populate_nkro_keyboard_report(&report.nkro);
		reportSize = sizeof(report.nkro);
	} else {
		shouldSend = USB_populate_boot_keyboard_report(&report.boot);
		reportSize = sizeof(report.boot);
	}

	if (USB_idle_timeout_duration && !USB_idle_timeout_remaining) {
		USB_idle_timeout_remaining = USB_idle_timeout_duration;
		shouldSend = true;
	}

	if (!shouldSend) {
		return;
	}

	USB_select_endpoint(USB_ENDPOINT_KEYBOARD_IN);
	USB_transfer_data(&report, reportSize, false, USB_ENDPOINT_KEYBOARD_SIZE);
}

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

USB_LedReport_t USB_update() {
	if (USB_DEVICE_STATE != USB_DeviceState_Configured) {
		return 0;
	}
	_HID_send_report();
}

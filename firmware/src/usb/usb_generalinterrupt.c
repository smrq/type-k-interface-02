#include "usb_generalinterrupt.h"

local void _USB_handle_connect() {
	USB_enable_pll();
	USBCON &= ~_BV(FRZCLK);

	USB_DEVICE_STATE = USB_DeviceState_Powered;

	/*
		When initialized, all devices default to report protocol.
			- Device Class Definition for HID 1.11, section 7.2.6, p. 54
	*/
	USB_using_report_protocol = true;
}

local void _USB_handle_disconnect() {
	USB_disable_pll();
	USB_DEVICE_STATE = USB_DeviceState_Detached;
}

local void _USB_handle_start_of_frame() {
	// TODO: This doesn't seem right, idle_timeout_remaining is measured in ms
	if (USB_idle_timeout_remaining) {
		USB_idle_timeout_remaining--;
	}
}

local void _USB_handle_end_of_reset() {
	USB_DEVICE_STATE = USB_DeviceState_Default;
	USB_configuration_number = 0;
	UDINT &= ~_BV(SUSPI);
	UDIEN &= ~_BV(SUSPE);
	UDIEN |= _BV(WAKEUPE);
	USB_configure_endpoint(USB_ENDPOINT_CONTROL,
		USB_CFG0X_ENDPOINT_TYPE_CONTROL | USB_CFG0X_ENDPOINT_DIRECTION_OUT,
		USB_CFG1X_ENDPOINT_SIZE(USB_ENDPOINT_CONTROL_SIZE));
	UEIENX |= _BV(RXSTPE);
}

local void _USB_handle_suspend() {
	/*
		Suspending the USB interface
		- Clear Suspend Bit
		- Freeze USB clock
		- Disable PLL
		- Be sure to have interrupts enable to exit sleep mode
		- Make the MCU enter sleep mode
			- ATmega32U4 datasheet, section 21.12 USB Software Operating Modes, p. 266
	*/
	USBCON |= _BV(FRZCLK);
	USB_disable_pll();
	UDIEN &= ~_BV(SUSPE);
	UDIEN |= _BV(WAKEUPE);
	USB_DEVICE_STATE = USB_DeviceState_Suspended;
}

local void _USB_handle_resume() {
	/*
		Resuming the USB interface
		- Enable PLL
		- Wait PLL lock
		- Unfreeze USB clock
		- Clear Resume information
			- ATmega32U4 datasheet, section 21.12 USB Software Operating Modes, p. 266
	*/
	USB_enable_pll();
	USBCON &= ~_BV(FRZCLK);
	UDINT &= ~_BV(WAKEUPI);
	UDIEN &= ~_BV(WAKEUPE);
	UDIEN |= _BV(SUSPE);

	if (USB_configuration_number) {
		USB_DEVICE_STATE = USB_DeviceState_Configured;
	} else if (UDADDR & _BV(ADDEN)) {
		USB_DEVICE_STATE = USB_DeviceState_Address;
	} else {
		USB_DEVICE_STATE = USB_DeviceState_Powered;
	}
}

ISR(USB_GEN_vect, ISR_BLOCK) {
	/*
		Note that for all of these interrupts, we check whether the relevant
		interrupt is enabled as well as triggered. This is because it is possible
		for an interrupt condition to be signaled when we are intentionally not
		listening to that interrupt. If another interrupt triggers the handler
		while this condition is true, then we would erroneously handle both.
	*/

	/*
		VBUSTI: IVBUS Transition Interrupt Flag
		Set by hardware when a transition (high to low, low to high) has been detected on the VBUS pad. This shall be
		cleared by software
			- ATmega32U4 datasheet, p. 268
	*/
	if ((USBINT & _BV(VBUSTI)) && (USBCON & _BV(VBUSTE))) {
		USBINT &= ~_BV(VBUSTI);
		if (USBSTA & _BV(VBUS)) {
			_USB_handle_connect();
		} else {
			_USB_handle_disconnect();
		}
	}

	/*
		SUSPI - Suspend Interrupt Flag
		Shall be cleared by software. Setting by software has no effect. The interrupt bits are set even if their
		corresponding ‘Enable’ bits is not set.
			- ATmega32U4 datasheet, p. 282
	*/
	if ((UDINT & _BV(SUSPI)) && (UDIEN & _BV(SUSPE))) {
		UDINT &= ~_BV(SUSPI);
		_USB_handle_suspend();
	}

	/*
		SOFI - Start Of Frame Interrupt Flag
		Set by hardware when an USB “Start Of Frame” PID (SOF) has been detected (every 1ms). This triggers an
		USB interrupt if SOFE is set.
			- ATmega32U4 datasheet, p. 282
	*/
	if ((UDINT & _BV(SOFI)) && (UDIEN & _BV(SOFE))) {
		UDINT &= ~_BV(SOFI);
		_USB_handle_start_of_frame();
	}

	/*
		EORSTI - End Of Reset Interrupt Flag
		Set by hardware when an “End Of Reset” has been detected by the USB controller. This triggers an USB
		interrupt if EORSTE is set.
		Shall be cleared by software. Setting by software has no effect.
			- ATmega32U4 datasheet, p. 282
	*/
	if ((UDINT & _BV(EORSTI)) && (UDIEN & _BV(EORSTE))) {
		UDINT &= ~_BV(EORSTI);
		_USB_handle_end_of_reset();
	}

	/*
		WAKEUPI - Wake-up CPU Interrupt Flag
		Set by hardware when the USB controller is re-activated by a filtered non-idle signal from the lines (not by an
		upstream resume). This triggers an interrupt if WAKEUPE is set.
		Shall be cleared by software (USB clock inputs must be enabled before). Setting by software has no effect.
			- ATmega32U4 datasheet, p. 282
	*/
	if ((UDINT & _BV(WAKEUPI)) && (UDIEN & _BV(WAKEUPE))) {
		_USB_handle_resume();
	}
}

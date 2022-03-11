#include "usb_hardware.h"

void USB_enable_pll() {
	/*
		When the PLOCK bit is set, the PLL is locked to the reference clock. After the PLL
		is enabled, it takes about several ms for the PLL to lock.
			- ATmega32U4 datasheet, p. 41
	*/
	PLLCSR = USB_PLL_PRESCALE;
	PLLCSR = USB_PLL_PRESCALE | _BV(PLLE);
	while (!(PLLCSR & _BV(PLOCK)));
}

void USB_disable_pll() {
	PLLCSR = 0;
}

void USB_set_address(u8 address) {
	/*
		Bit 7 - ADDEN - Address Enable Bit
			(...) Cleared by hardware. Clearing by software has no effect.
		Bits 6-0 - UADD6:0 - USB Address Bits
			Load by software to configure the device address
			- ATmega32U4 datasheet, p. 284
	*/
	UDADDR = address & 0x7F;
}

void USB_enable_address() {
	/*
		Bit 7 - ADDEN - Address Enable Bit
			Set to activate the UADD (USB address).
			- ATmega32U4 datasheet, p. 284
	*/
	UDADDR |= _BV(ADDEN);
}

/* ATmega32U4 datasheet, section 22.6 Endpoint Activation, p. 271 */
bool USB_configure_endpoint(u8 number, u8 flags0, u8 flags1) {
	UENUM = number;
	UECONX = _BV(EPEN);
	UECFG0X = flags0;
	UECFG1X = flags1 | _BV(ALLOC);
	return UESTA0X & _BV(CFGOK);
}

void USB_reset_endpoint(u8 number) {
	/*
		Bits 6-0 - EPRST6:0 - Endpoint FIFO Reset Bits
		Set to reset the selected endpoint FIFO prior to any other operation, upon hardware reset or when an USB bus
		reset has been received. See “Endpoint Reset” on page 270 for more information
		Then, clear by software to complete the reset operation and start using the endpoint.
			- ATmega32U4 datasheet, p. 285
	*/
	UERST = _BV(number);
	UERST = 0;
}

void USB_reset_endpoint_data_toggle() {
	UECONX |= _BV(RSTDT);
}

bool USB_is_endpoint_enabled() {
	return UECONX & _BV(EPEN);
}

void USB_select_endpoint(u8 number) {
	UENUM = number;
}

u8 USB_get_selected_endpoint() {
	return UENUM;
}

u8 USB_read_byte_from_endpoint() {
	return UEDATX;
}

void USB_read_from_endpoint(void *buffer, u16 length) {
	for (u8 *ptr = (u8 *)buffer; ptr < (u8 *)buffer + length; ++ptr) {
		*ptr = UEDATX;
	}
}

void USB_write_byte_to_endpoint(u8 byte) {
	UEDATX = byte;
}

u16 USB_bytes_in_endpoint() {
	return ((u16)UEBCHX << 8) | UEBCLX;
}

void USB_stall_endpoint() {
	UECONX |= _BV(STALLRQ);
}

void USB_clear_endpoint_stall() {
	UECONX |= _BV(STALLRQC);
}

bool USB_is_endpoint_stalled() {
	return UECONX & _BV(STALLRQ);
}

bool USB_is_in_ready() {
	return UEINTX & _BV(TXINI);
}

bool USB_wait_for_in_ready() {
	while (!USB_is_in_ready()) {
		if (USB_DEVICE_STATE == USB_DeviceState_Detached ||
			USB_DEVICE_STATE == USB_DeviceState_Suspended ||
			USB_is_endpoint_stalled()) {
			return false;
		}
	}
	return true;
}

void USB_send_in() {
	UEINTX &= ~(_BV(TXINI) | _BV(FIFOCON));
}

bool USB_is_out_ready() {
	return UEINTX & _BV(RXOUTI);
}

bool USB_wait_for_out_ready() {
	while (!USB_is_out_ready()) {
		if (USB_DEVICE_STATE == USB_DeviceState_Detached ||
			USB_DEVICE_STATE == USB_DeviceState_Suspended ||
			USB_is_endpoint_stalled()) {
			return false;
		}
	}
	return true;
}

void USB_clear_out() {
	UEINTX &= ~(_BV(RXOUTI) | _BV(FIFOCON));
}

bool USB_is_setup_ready() {
	return UEINTX & _BV(RXSTPI);
}

void USB_clear_setup() {
	UEINTX &= ~_BV(RXSTPI);
}

bool USB_transfer_data(const void *buffer, u16 length, bool fromProgmem, u8 endpointSize) {
	do {
		while (!USB_is_in_ready()) {
			if (USB_DEVICE_STATE == USB_DeviceState_Detached ||
				USB_DEVICE_STATE == USB_DeviceState_Suspended ||
				USB_is_endpoint_stalled() ||
				USB_is_setup_ready()) {
				return false;
			}
			if (USB_is_out_ready()) {
				break;
			}
		}

		if (length) {
			u16 remainingSpace = endpointSize - USB_bytes_in_endpoint();
			while (length && remainingSpace) {
				USB_write_byte_to_endpoint(fromProgmem ? pgm_read_byte((u8 *)buffer++) : *(u8 *)buffer++);
				length--;
				remainingSpace--;
			}
		}

		USB_send_in();
	} while (length);

	return true;
}

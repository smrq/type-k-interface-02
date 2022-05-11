#pragma once

#include "../defs.h"
#include "usb_defs.h"

#define USB_DEVICE_STATE GPIOR0

#if (F_CPU == 8000000)
	#define USB_PLL_PRESCALE 0
#elif (F_CPU == 16000000)
	#define USB_PLL_PRESCALE _BV(PINDIV)
#endif

#define USB_ENDPOINT_CONTROL       0
#define USB_ENDPOINT_CONTROL_SIZE  64

/* ATmega32U4 datasheet, p. 287 */
#define USB_CFG0X_ENDPOINT_DIRECTION_OUT (0 << EPDIR)
#define USB_CFG0X_ENDPOINT_DIRECTION_IN  (1 << EPDIR)

#define USB_CFG0X_ENDPOINT_TYPE_CONTROL     (0 << EPTYPE0)
#define USB_CFG0X_ENDPOINT_TYPE_ISOCHRONOUS (1 << EPTYPE0)
#define USB_CFG0X_ENDPOINT_TYPE_BULK        (2 << EPTYPE0)
#define USB_CFG0X_ENDPOINT_TYPE_INTERRUPT   (3 << EPTYPE0)

#define USB_CFG1X_ENDPOINT_SIZE_8      (0 << EPSIZE0)
#define USB_CFG1X_ENDPOINT_SIZE_16     (1 << EPSIZE0)
#define USB_CFG1X_ENDPOINT_SIZE_32     (2 << EPSIZE0)
#define USB_CFG1X_ENDPOINT_SIZE_64     (3 << EPSIZE0)
#define USB_CFG1X_ENDPOINT_SIZE_128    (4 << EPSIZE0)
#define USB_CFG1X_ENDPOINT_SIZE_256    (5 << EPSIZE0)
#define USB_CFG1X_ENDPOINT_SIZE_512    (6 << EPSIZE0)
#define _USB_CFG1X_ENDPOINT_SIZE(bytes) USB_CFG1X_ENDPOINT_SIZE_##bytes
#define USB_CFG1X_ENDPOINT_SIZE(bytes) _USB_CFG1X_ENDPOINT_SIZE(bytes)

void USB_enable_pll();
void USB_disable_pll();
void USB_set_address(u8 address);
void USB_enable_address();
bool USB_configure_endpoint(u8 number, u8 flags0, u8 flags1);
void USB_reset_endpoint(u8 number);
void USB_reset_endpoint_data_toggle();
bool USB_is_endpoint_enabled();
void USB_select_endpoint(u8 number);
u8 USB_get_selected_endpoint();
u8 USB_read_byte_from_endpoint();
void USB_read_from_endpoint(void *buffer, u16 length);
void USB_write_byte_to_endpoint(u8 byte);
u16 USB_bytes_in_endpoint();
void USB_stall_endpoint();
void USB_clear_endpoint_stall();
bool USB_is_endpoint_stalled();
bool USB_is_in_ready();
bool USB_wait_for_in_ready();
void USB_send_in();
bool USB_is_out_ready();
bool USB_wait_for_out_ready();
void USB_clear_out();
bool USB_is_setup_ready();
void USB_clear_setup();
bool USB_transfer_data(const void *buffer, u16 length, bool fromProgmem, u8 endpointSize);
void USB_send_remote_wakeup();

#pragma once

#include "../defs.h"
#include "usb_spec.h"

/*
	A device can have more than one configuration, though only one at a time,
	and to change configuration the whole device would have to stop functioning.
	Different configurations might be used, for example, to specify different
	current requirements, as the current required is defined in the configuration
	descriptor.

	However it is not common to have more than one configuration. Windows standard
	drivers will always select the first configuration so there is not a lot of point.

	- http://www.usbmadesimple.co.uk/ums_4.htm
*/
#define USB_CONFIGURATION_COUNT    1
#define USB_CONFIGURATION_KEYBOARD 1

#define USB_INTERFACE_COUNT        2
#define USB_INTERFACE_KEYBOARD     1
#define USB_INTERFACE_OTHER        2

/*
	We don't need to define OUT endpoints for HID devices:

	The Interrupt Out pipe is optional. If a device declares an Interrupt Out endpoint
	then Output reports are transmitted by the host to the device through the Interrupt
	Out endpoint. If no Interrupt Out endpoint is declared then Output reports are
	transmitted to a device through the Control endpoint, using Set_Report(Output)
	requests.

	- USB HID 1.11 Specification, section 4.4 Interfaces, p. 10
*/
#define USB_ENDPOINT_COUNT         2
#define USB_ENDPOINT_KEYBOARD_IN   1
#define USB_ENDPOINT_OTHER_IN      2

#define USB_ENDPOINT_KEYBOARD_SIZE 32
#define USB_ENDPOINT_OTHER_SIZE    8

#define USB_REPORT_ID_SYSTEM       1
#define USB_REPORT_ID_CONSUMER     2

#define USB_VENDOR_ID              0xF055
#define USB_PRODUCT_ID             0x4B32
#define USB_RELEASE_NUMBER         BCD(1,0,0)

#define USB_MANUFACTURER_NAME      L"smrq"
#define USB_PRODUCT_NAME           L"TYPE-K Interface 02"

typedef struct {
	u8 keys[32];
} PACKED USB_NkroKeyboardReport_t;

typedef struct {
	u8 modifiers;
	u8 reserved;
	u8 keyCodes[6];
} PACKED USB_BootKeyboardReport_t;

typedef u8 USB_LedReport_t;

typedef struct {
	u8 reportId;
	u16 code;
} PACKED USB_OtherReport_t;

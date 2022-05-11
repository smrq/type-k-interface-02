#include "usb_descriptors.h"

typedef struct {
	USB_ConfigurationDescriptor_t configuration;
	USB_InterfaceDescriptor_t     keyboardInterface;
	HID_Descriptor_t              keyboardHid;
	USB_EndpointDescriptor_t      keyboardInEndpoint;
	USB_InterfaceDescriptor_t     otherInterface;
	HID_Descriptor_t              otherHid;
	USB_EndpointDescriptor_t      otherInEndpoint;
} USB_CombinedConfigurationDescriptor_t;

enum StringDescriptorId_t {
	StringDescriptorId_Language     = 0,
	StringDescriptorId_Manufacturer = 1,
	StringDescriptorId_Product      = 2
};

/*
	Note that the key report is split into several similar items, because:

	An item field cannot span more than 4 bytes in a report. For example, a 32-bit
	item must start on a byte boundary to satisfy this condition.
		- USB HID 1.11 Specification, p. 57
*/
local const PROGMEM u8 _nkroKeyboardReportDescriptor[] = {
	HID_USAGE_PAGE      (8, HID_USAGE_PAGE_GENERIC_DESKTOP),
	HID_USAGE           (8, HID_DESKTOP_KEYBOARD),
	HID_COLLECTION      (8, HID_COLLECTION_APPLICATION),

	HID_USAGE_PAGE      (8, HID_USAGE_PAGE_KEYBOARD_KEYPAD),
	HID_LOGICAL_MINIMUM (8, 0x00),
	HID_LOGICAL_MAXIMUM (8, 0x01),
	HID_REPORT_SIZE     (8, 0x01),
	HID_REPORT_COUNT    (8, 0x20),
	HID_USAGE_MINIMUM   (8, 0x00),
	HID_USAGE_MAXIMUM   (8, 0x1F),
	HID_INPUT           (8, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

	HID_USAGE_MINIMUM   (8, 0x20),
	HID_USAGE_MAXIMUM   (8, 0x3F),
	HID_INPUT           (8, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

	HID_USAGE_MINIMUM   (8, 0x40),
	HID_USAGE_MAXIMUM   (8, 0x5F),
	HID_INPUT           (8, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

	HID_USAGE_MINIMUM   (8, 0x60),
	HID_USAGE_MAXIMUM   (8, 0x7F),
	HID_INPUT           (8, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

	HID_USAGE_MINIMUM   (8, 0x80),
	HID_USAGE_MAXIMUM   (8, 0x9F),
	HID_INPUT           (8, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

	HID_USAGE_MINIMUM   (8, 0xA0),
	HID_USAGE_MAXIMUM   (8, 0xBF),
	HID_INPUT           (8, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

	HID_USAGE_MINIMUM   (8, 0xC0),
	HID_USAGE_MAXIMUM   (8, 0xDF),
	HID_INPUT           (8, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

	HID_USAGE_MINIMUM   (8, 0xE0),
	HID_USAGE_MAXIMUM   (8, 0xFF),
	HID_INPUT           (8, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

	HID_USAGE_PAGE      (8, HID_USAGE_PAGE_LED),
	HID_USAGE_MINIMUM   (8, 0x01),
	HID_USAGE_MAXIMUM   (8, 0x05),
	HID_REPORT_SIZE     (8, 0x01),
	HID_REPORT_COUNT    (8, 0x05),
	HID_OUTPUT          (8, HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

	HID_REPORT_SIZE     (8, 0x03),
	HID_REPORT_COUNT    (8, 0x01),
	HID_OUTPUT          (8, HID_CONSTANT),

	HID_END_COLLECTION  (0)
};

local const PROGMEM u8 _otherReportDescriptor[] = {
	HID_USAGE_PAGE      (8, HID_USAGE_PAGE_GENERIC_DESKTOP),
	HID_USAGE           (8, HID_DESKTOP_SYSTEM_CONTROL),
	HID_COLLECTION      (8, HID_COLLECTION_APPLICATION),

	HID_REPORT_ID       (8, USB_REPORT_ID_SYSTEM),
	HID_LOGICAL_MINIMUM (8, 0x01),
	HID_LOGICAL_MAXIMUM (8, 0xB7),
	HID_USAGE_MINIMUM   (8, 0x01),
	HID_USAGE_MAXIMUM   (8, 0xB7),
	HID_REPORT_SIZE     (8, 0x10),
	HID_REPORT_COUNT    (8, 0x01),
	HID_INPUT           (8, HID_DATA | HID_ARRAY | HID_ABSOLUTE),

	HID_END_COLLECTION  (0),

	HID_USAGE_PAGE      (8, HID_USAGE_PAGE_CONSUMER),
	HID_USAGE           (8, HID_CONSUMER_CONTROL),
	HID_COLLECTION      (8, HID_COLLECTION_APPLICATION),

	HID_REPORT_ID       (8, USB_REPORT_ID_CONSUMER),
	HID_LOGICAL_MINIMUM (8, 0x01),
	HID_LOGICAL_MAXIMUM (16, 0x0514),
	HID_USAGE_MINIMUM   (8, 0x01),
	HID_USAGE_MAXIMUM   (16, 0x0514),
	HID_REPORT_SIZE     (8, 0x10),
	HID_REPORT_COUNT    (8, 0x01),
	HID_INPUT           (8, HID_DATA | HID_ARRAY | HID_ABSOLUTE),

	HID_END_COLLECTION  (0)
};

local const PROGMEM USB_DeviceDescriptor_t _deviceDescriptor = {
	.header = {
		.length         = sizeof(USB_DeviceDescriptor_t),
		.descriptorType = USB_DescriptorType_Device,
	},
	.usbVersion         = BCD(1,1,0),
	.deviceClass        = USB_Class_FromInterface,
	.deviceSubClass     = USB_Subclass_FromInterface,
	.deviceProtocol     = USB_Protocol_FromInterface,
	.maxPacketSize      = USB_ENDPOINT_CONTROL_SIZE,
	.vendorId           = USB_VENDOR_ID,
	.productId          = USB_PRODUCT_ID,
	.deviceVersion      = USB_RELEASE_NUMBER,
	.manufacturerString = StringDescriptorId_Manufacturer,
	.productString      = StringDescriptorId_Product,
	.serialNumberString = USB_NO_STRING_DESCRIPTOR,
	.numConfigurations  = USB_CONFIGURATION_COUNT
};

local const PROGMEM USB_CombinedConfigurationDescriptor_t _configurationDescriptor = {
	.configuration = {
		.header = {
			.length          = sizeof(USB_ConfigurationDescriptor_t),
			.descriptorType  = USB_DescriptorType_Configuration
		},
		.totalLength         = sizeof(USB_CombinedConfigurationDescriptor_t),
		.numInterfaces       = 2,
		.configurationValue  = USB_CONFIGURATION_KEYBOARD,
		.configurationString = USB_NO_STRING_DESCRIPTOR,
		.attributes          = (USB_CONFIGURATION_ATTRIBUTES_RESERVED | USB_CONFIGURATION_ATTRIBUTES_SELF_POWERED | USB_CONFIGURATION_ATTRIBUTES_REMOTE_WAKEUP),
		.maxPower            = USB_MAX_POWER_mA(100) // I *definitely* promise not to use more than 100mA... per LED^H^H^H^H^H^H^H
	},

	.keyboardInterface = {
		.header = {
			.length         = sizeof(USB_InterfaceDescriptor_t),
			.descriptorType = USB_DescriptorType_Interface
		},
		.interfaceNumber    = USB_INTERFACE_KEYBOARD,
		.alternateSetting   = 0,
		.numEndpoints       = 1,
		.interfaceClass     = USB_Class_HID,
		.interfaceSubClass  = HID_Subclass_Boot,
		.interfaceProtocol  = HID_Protocol_Keyboard,
		.interfaceString    = USB_NO_STRING_DESCRIPTOR
	},

	.keyboardHid = {
		.header = {
			.length         = sizeof(HID_Descriptor_t),
			.descriptorType = HID_DescriptorType_HID,
		},
		.hidVersion         = BCD(1,1,1),
		.countryCode        = HID_CountryCode_NotSupported,
		.numDescriptors     = 1,
		.descriptorType     = HID_DescriptorType_Report,
		.descriptorLength   = sizeof(_nkroKeyboardReportDescriptor)
	},

	.keyboardInEndpoint = {
		.header = {
			.length         = sizeof(USB_EndpointDescriptor_t),
			.descriptorType = USB_DescriptorType_Endpoint
		},
		.endpointAddress    = USB_ENDPOINT_KEYBOARD_IN | USB_ENDPOINT_ADDRESS_DIRECTION_IN,
		.attributes         = USB_ENDPOINT_ATTRIBUTES_INTERRUPT,
		.maxPacketSize      = USB_ENDPOINT_KEYBOARD_SIZE,
		.interval           = 5
	},

	.otherInterface = {
		.header = {
			.length         = sizeof(USB_InterfaceDescriptor_t),
			.descriptorType = USB_DescriptorType_Interface
		},
		.interfaceNumber    = USB_INTERFACE_OTHER,
		.alternateSetting   = 0,
		.numEndpoints       = 1,
		.interfaceClass     = USB_Class_HID,
		.interfaceSubClass  = HID_Subclass_None,
		.interfaceProtocol  = HID_Protocol_None,
		.interfaceString    = USB_NO_STRING_DESCRIPTOR
	},

	.otherHid = {
		.header = {
			.length         = sizeof(HID_Descriptor_t),
			.descriptorType = HID_DescriptorType_HID,
		},
		.hidVersion         = BCD(1,1,1),
		.countryCode        = HID_CountryCode_NotSupported,
		.numDescriptors     = 1,
		.descriptorType     = HID_DescriptorType_Report,
		.descriptorLength   = sizeof(_otherReportDescriptor)
	},

	.otherInEndpoint = {
		.header = {
			.length         = sizeof(USB_EndpointDescriptor_t),
			.descriptorType = USB_DescriptorType_Endpoint
		},
		.endpointAddress    = USB_ENDPOINT_OTHER_IN | USB_ENDPOINT_ADDRESS_DIRECTION_IN,
		.attributes         = USB_ENDPOINT_ATTRIBUTES_INTERRUPT,
		.maxPacketSize      = USB_ENDPOINT_OTHER_SIZE,
		.interval           = 5
	}
};

local const PROGMEM USB_LanguageDescriptor_t      _languageDescriptor     = USB_LANGUAGE_DESCRIPTOR(USB_LANGUAGE_ID_ENGLISH_US);
local const PROGMEM USB_UnicodeStringDescriptor_t _manufacturerDescriptor = USB_UNICODE_STRING_DESCRIPTOR(USB_MANUFACTURER_NAME);
local const PROGMEM USB_UnicodeStringDescriptor_t _productDescriptor      = USB_UNICODE_STRING_DESCRIPTOR(USB_PRODUCT_NAME);

bool USB_get_descriptor(u8 type, u8 number, u16 index, const void **address, u16 *size) {
	switch (type) {
		case USB_DescriptorType_Device:
			*address = &_deviceDescriptor;
			*size = sizeof(_deviceDescriptor);
			return true;

		case USB_DescriptorType_Configuration:
			*address = &_configurationDescriptor;
			*size = sizeof(_configurationDescriptor);
			return true;

		case USB_DescriptorType_String:
			switch (number) {
				case StringDescriptorId_Language:
					*address = &_languageDescriptor;
					*size = sizeof(_languageDescriptor);
					return true;

				case StringDescriptorId_Manufacturer:
					*address = &_manufacturerDescriptor;
					*size = sizeof(_manufacturerDescriptor);
					return true;

				case StringDescriptorId_Product:
					*address = &_productDescriptor;
					*size = sizeof(_productDescriptor);
					return true;

				default:
					return false;
			}

		case HID_DescriptorType_HID:
			switch (index) {
				case USB_INTERFACE_KEYBOARD:
					*address = &_configurationDescriptor.keyboardHid;
					*size = sizeof(_configurationDescriptor.keyboardHid);
					return true;

				case USB_INTERFACE_OTHER:
					*address = &_configurationDescriptor.otherHid;
					*size = sizeof(_configurationDescriptor.otherHid);
					return true;

				default:
					return false;
			}

		case HID_DescriptorType_Report:
			switch (index) {
				case USB_INTERFACE_KEYBOARD:
					*address = &_nkroKeyboardReportDescriptor;
					*size = sizeof(_nkroKeyboardReportDescriptor);
					return true;

				case USB_INTERFACE_OTHER:
					*address = &_otherReportDescriptor;
					*size = sizeof(_otherReportDescriptor);
					return true;

				default:
					while(1);
					return false;
			}

		default:
			return false;
	}
}

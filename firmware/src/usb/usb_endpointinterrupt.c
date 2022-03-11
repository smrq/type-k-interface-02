#include "usb_endpointinterrupt.h"

/*
	Note for all control requests:

	The Status stage of a control transfer is the last operation in the sequence. A Status stage is delineated by a
	change in direction of data flow from the previous stage and always uses a DATA1 PID. If, for example,
	the Data stage consists of OUTs, the status is a single IN transaction. If the control sequence has no Data
	stage, then it consists of a Setup stage followed by a Status stage consisting of an IN transaction.
		- USB 1.1 Specification, section 8.5.2 Control Transfers, p. 165
*/

/* USB 1.1 Specification, section 9.4.6 Set Address, p. 192 */
local bool _USB_handle_set_address(u8 value) {
	/*
		If the specified device address is greater than 127 (...) then the behavior of the device is not specified.
		Device response to SetAddress() with a value of 0 is undefined.
			- USB 1.1 Specification, section 9.4.6 Set Address, p. 192
	*/
	u8 deviceAddress = value & 0x7F;

	/*
		The USB device address is set up according to the USB protocol:
		- the USB device, after power-up, responds at address 0
		- the host sends a SETUP command (SET_ADDRESS(addr))
		- the firmware handles this request, and records that address in UADD, but keep ADDEN cleared
		- the USB device firmware sends an IN command of 0 bytes (IN 0 Zero Length Packet)
		- then, the firmware can enable the USB device address by setting ADDEN. The only accepted address by
		  the controller is the one stored in UADD.
		ADDEN and UADD shall not be written at the same time.
			- ATmega32U4 datasheet, section 22.7 Address Setup, p. 272

		The USB device does not change its device address until after the Status stage of this request is
		completed successfully. Note that this is a difference between this request and all other requests.
			- USB 1.1 Specification, section 9.4.6 Set Address, p. 192
	*/
	USB_set_address(deviceAddress);

	if (!USB_wait_for_in_ready()) return false;
	USB_send_in();

	if (!USB_wait_for_in_ready()) return false;
	USB_enable_address();
	USB_DEVICE_STATE = USB_DeviceState_Address;

	return true;
}

/* USB 1.1 Specification, section 9.4.2 Get Configuration, p. 189 */
local bool _USB_handle_get_configuration() {
	/*
		Default state: Device behavior when this request is received while the device is in the Default state is not specified.
		Address state: The value zero shall be returned.
		Configured state: The non-zero bConfigurationNumber of the current configuration shall be returned.
			- USB 1.1 Specification, section 9.4.2 Get Configuration, p. 189
	*/
	u8 value;
	switch (USB_DEVICE_STATE) {
		case USB_DeviceState_Address:
			value = 0;
			break;

		case USB_DeviceState_Configured:
			value = USB_configuration_number;
			break;

		default:
			return false;
	}

	if (!USB_wait_for_in_ready()) return false;
	USB_write_byte_to_endpoint(value);
	USB_send_in();

	if (!USB_wait_for_out_ready()) return false;
	USB_clear_out();

	return true;
}

/* USB 1.1 Specification, section 9.4.7 Set Configuration, p. 193 */
local bool _USB_handle_set_configuration(u8 value) {
	/*
		The lower byte of the wValue field specifies the desired configuration. This configuration value must be
		zero or match a configuration value from a configuration descriptor. If the configuration value is zero, the
		device is placed in its Address state.

		Default state: Device behavior when this request is received while the device is in the Default state
			is not specified.
		Address state: If the specified configuration value is zero, then the device remains in the Address
			state. If the specified configuration value matches the configuration value from a
			configuration descriptor, then that configuration is selected and the device enters the
			Configured state. Otherwise, the device responds with a Request Error.
		Configured state: If the specified configuration value is zero, then the device enters the Address state.
			If the specified configuration value matches the configuration value from a
			configuration descriptor, then that configuration is selected and the device remains in
			the Configured state. Otherwise, the device responds with a Request Error.
			- USB 1.1 Specification, section 9.4.7 Set Configuration, p. 193
	*/
	if (value > USB_CONFIGURATION_COUNT) {
		return false;
	}

	USB_configuration_number = value;

	if (value) {
		USB_DEVICE_STATE = USB_DeviceState_Configured;
		USB_configure_endpoint(USB_ENDPOINT_KEYBOARD_IN,
			USB_CFG0X_ENDPOINT_TYPE_INTERRUPT | USB_CFG0X_ENDPOINT_DIRECTION_IN,
			USB_CFG1X_ENDPOINT_SIZE(USB_ENDPOINT_KEYBOARD_SIZE));
		USB_select_endpoint(USB_ENDPOINT_CONTROL);
		UDIEN |= _BV(SOFE);
	} else {
		USB_DEVICE_STATE = USB_DeviceState_Address;
		UDIEN &= _BV(SOFE);
	}

	if (!USB_wait_for_in_ready()) return false;
	USB_send_in();

	return true;
}

/* USB 1.1 Specification, section 9.4.3 Get Descriptor, p. 189 */
local bool _USB_handle_get_descriptor(u16 value, u8 length) {
	u8 descriptorType = (value >> 8) & 0xFF;
	u8 descriptorIndex = (value >> 0) & 0xFF;

	const void *descriptorAddress;
	u16 descriptorSize;
	if (!USB_get_descriptor(descriptorType, descriptorIndex, &descriptorAddress, &descriptorSize)) {
		return false;
	}

	/*
		If the descriptor is longer than the wLength field, only the initial bytes of the descriptor are returned.
			- USB 1.1 Specification, section 9.4.3 Get Descriptor, p. 189
	*/
	if (descriptorSize > length) {
		descriptorSize = length;
	}

	if (!USB_transfer_data(descriptorAddress, descriptorSize, true, USB_ENDPOINT_CONTROL_SIZE)) {
		return false;
	}

	if (!USB_wait_for_out_ready()) return false;
	USB_clear_out();

	return true;
}

/* USB 1.1 Specification, section 9.4.1 Clear Feature, p. 188 */
local bool _USB_handle_clear_device_feature(u8 value) {
	switch (value) {
		case USB_FeatureSelector_DeviceRemoteWakeup:
			USB_remote_wakeup_enabled = false;
			break;

		default:
			return false;
	}

	if (!USB_wait_for_in_ready()) return false;
	USB_send_in();

	return true;
}

/* USB 1.1 Specification, section 9.4.9 Set Feature, p. 194 */
local bool _USB_handle_set_device_feature(u8 value) {
	switch (value) {
		case USB_FeatureSelector_DeviceRemoteWakeup:
			USB_remote_wakeup_enabled = true;
			break;

		default:
			return false;
	}

	if (!USB_wait_for_in_ready()) return false;
	USB_send_in();

	return true;
}

/* USB 1.1 Specification, section 9.4.1 Clear Feature, p. 188 */
local bool _USB_handle_clear_endpoint_feature(u8 value, u8 index) {
	u8 endpointNumber = index & USB_REQUEST_INDEX_ENDPOINT_NUMBER_MASK;

	switch (value) {
		case USB_FeatureSelector_EndpointHalt:
			if (endpointNumber == USB_ENDPOINT_CONTROL) {
				return false;
			}

			if (endpointNumber >= USB_ENDPOINT_COUNT) {
				return false;
			}

			USB_select_endpoint(endpointNumber);
			if (USB_is_endpoint_enabled()) {
				USB_clear_endpoint_stall();
				USB_reset_endpoint(endpointNumber);

				/*
					For endpoints using data toggle, regardless of whether an endpoint has the Halt feature set, a
					ClearFeature(ENDPOINT_HALT) request always results in the data toggle being reinitialized to DATA0.
						- USB 1.1 Specification, p. 192
				*/
				USB_reset_endpoint_data_toggle();
			}
			USB_select_endpoint(USB_ENDPOINT_CONTROL);
			break;

		default:
			return false;
	}

	if (!USB_wait_for_in_ready()) return false;
	USB_send_in();

	return true;
}

/* USB 1.1 Specification, section 9.4.9 Set Feature, p. 194 */
local bool _USB_handle_set_endpoint_feature(u8 value, u8 index) {
	u8 endpointNumber = index & USB_REQUEST_INDEX_ENDPOINT_NUMBER_MASK;

	switch (value) {
		case USB_FeatureSelector_EndpointHalt:
			if (endpointNumber == USB_ENDPOINT_CONTROL) {
				return false;
			}

			if (endpointNumber >= USB_ENDPOINT_COUNT) {
				return false;
			}

			USB_select_endpoint(endpointNumber);
			if (USB_is_endpoint_enabled()) {
				USB_stall_endpoint();
			}
			USB_select_endpoint(USB_ENDPOINT_CONTROL);

			break;

		default:
			return false;
	}

	if (!USB_wait_for_in_ready()) return false;
	USB_send_in();

	return true;
}

/* USB 1.1 Specification, section 9.4.5 Get Status, p. 190 */
local bool _USB_handle_get_device_status() {
	u8 status = (USB_remote_wakeup_enabled ? _BV(1) : 0);

	if (!USB_wait_for_in_ready()) return false;
	USB_write_byte_to_endpoint(status);
	USB_write_byte_to_endpoint(0);
	USB_send_in();

	if (!USB_wait_for_out_ready()) return false;
	USB_clear_out();

	return true;
}

/* USB 1.1 Specification, section 9.4.5 Get Status, p. 190 */
local bool _USB_handle_get_endpoint_status(u8 index) {
	u8 endpointNumber = index & USB_REQUEST_INDEX_ENDPOINT_NUMBER_MASK;

	if (endpointNumber >= USB_ENDPOINT_COUNT) {
		return false;
	}

	USB_select_endpoint(endpointNumber);
	u8 status = USB_is_endpoint_stalled() ? _BV(0) : 0;
	USB_select_endpoint(USB_ENDPOINT_CONTROL);

	if (!USB_wait_for_in_ready()) return false;
	USB_write_byte_to_endpoint(status);
	USB_write_byte_to_endpoint(0);
	USB_send_in();

	if (!USB_wait_for_out_ready()) return false;
	USB_clear_out();

	return true;
}

/* USB HID 1.11 Specification, section 7.2.1 Get_Report Request, p. 51 */
local bool _USB_handle_get_report(u8 length) {
	union {
		USB_BootKeyboardReport_t boot;
		USB_NkroKeyboardReport_t nkro;
	} report = { 0 };
	u8 reportSize;

	if (USB_using_report_protocol) {
		USB_populate_nkro_keyboard_report(&report.nkro);
		reportSize = sizeof(report.nkro);
	} else {
		USB_populate_boot_keyboard_report(&report.boot);
		reportSize = sizeof(report.boot);
	}

	if (reportSize > length) {
		reportSize = length;
	}

	USB_transfer_data(&report, reportSize, false, USB_ENDPOINT_CONTROL_SIZE);

	if (!USB_wait_for_out_ready()) return false;
	USB_clear_out();

	return true;
}

/* USB HID 1.11 Specification, section 7.2.2 Set_Report Request, p. 52 */
local bool _USB_handle_set_report() {
	if (!USB_wait_for_out_ready()) return false;

	USB_LedReport_t report = USB_read_byte_from_endpoint();
	USB_clear_out();

	if (!USB_wait_for_in_ready()) return false;
	USB_send_in();

	USB_process_led_report(report);

	return true;
}

/* USB HID 1.11 Specification, section 7.2.3 Get_Idle Request, p. 52 */
local bool _USB_handle_get_idle() {
	if (!USB_wait_for_in_ready()) return false;
	USB_write_byte_to_endpoint(USB_idle_timeout_duration >> 2);
	USB_send_in();

	if (!USB_wait_for_out_ready()) return false;
	USB_clear_out();

	return true;
}

/* USB HID 1.11 Specification, section 7.2.4 Set_Idle Request, p. 52 */
local bool _USB_handle_set_idle(u8 value) {
	USB_idle_timeout_duration = (value & 0xFF00) >> 6;

	if (!USB_wait_for_in_ready()) return false;
	USB_send_in();

	return true;
}

/* USB HID 1.11 Specification, section 7.2.5 Get_Protocol Request, p. 54 */
local bool _USB_handle_get_protocol() {
	if (!USB_wait_for_in_ready()) return false;
	USB_write_byte_to_endpoint(USB_using_report_protocol);
	USB_send_in();

	if (!USB_wait_for_out_ready()) return false;
	USB_clear_out();

	return true;
}

/* USB HID 1.11 Specification, section 7.2.6 Set_Protocol Request, p. 54 */
local bool _USB_handle_set_protocol(u8 value) {
	USB_using_report_protocol = value;

	if (!USB_wait_for_in_ready()) return false;
	USB_send_in();

	return true;
}

local void _USB_handle_control_request(USB_DeviceRequest_t *controlRequest) {
	bool handled = false;

	// These flags are soooo long.
	#define ATTR_D2H_STD_DEV (USB_REQUEST_ATTRIBUTE_DIRECTION_DEVICE_TO_HOST | USB_REQUEST_ATTRIBUTE_TYPE_STANDARD | USB_REQUEST_ATTRIBUTE_RECIPIENT_DEVICE)
	#define ATTR_H2D_STD_DEV (USB_REQUEST_ATTRIBUTE_DIRECTION_HOST_TO_DEVICE | USB_REQUEST_ATTRIBUTE_TYPE_STANDARD | USB_REQUEST_ATTRIBUTE_RECIPIENT_DEVICE)
	#define ATTR_D2H_STD_END (USB_REQUEST_ATTRIBUTE_DIRECTION_DEVICE_TO_HOST | USB_REQUEST_ATTRIBUTE_TYPE_STANDARD | USB_REQUEST_ATTRIBUTE_RECIPIENT_ENDPOINT)
	#define ATTR_H2D_STD_END (USB_REQUEST_ATTRIBUTE_DIRECTION_HOST_TO_DEVICE | USB_REQUEST_ATTRIBUTE_TYPE_STANDARD | USB_REQUEST_ATTRIBUTE_RECIPIENT_ENDPOINT)
	#define ATTR_D2H_STD_INT (USB_REQUEST_ATTRIBUTE_DIRECTION_DEVICE_TO_HOST | USB_REQUEST_ATTRIBUTE_TYPE_STANDARD | USB_REQUEST_ATTRIBUTE_RECIPIENT_INTERFACE)
	#define ATTR_D2H_CLS_INT (USB_REQUEST_ATTRIBUTE_DIRECTION_DEVICE_TO_HOST | USB_REQUEST_ATTRIBUTE_TYPE_CLASS | USB_REQUEST_ATTRIBUTE_RECIPIENT_INTERFACE)
	#define ATTR_H2D_CLS_INT (USB_REQUEST_ATTRIBUTE_DIRECTION_HOST_TO_DEVICE | USB_REQUEST_ATTRIBUTE_TYPE_CLASS | USB_REQUEST_ATTRIBUTE_RECIPIENT_INTERFACE)
	#define REQUEST(req, type) (((req) << 8) | (type))

	switch (*(u16 *)controlRequest) {
		/* USB 1.1 Specification, section 9.4.1 Clear Feature, p. 188 */
		case REQUEST(USB_RequestCode_ClearFeature, ATTR_H2D_STD_DEV):
			handled = _USB_handle_clear_device_feature(controlRequest->value);
			break;

		/* USB 1.1 Specification, section 9.4.1 Clear Feature, p. 188 */
		case REQUEST(USB_RequestCode_ClearFeature, ATTR_H2D_STD_END):
			handled = _USB_handle_clear_endpoint_feature(controlRequest->value, controlRequest->index);
			break;

		/* USB 1.1 Specification, section 9.4.2 Get Configuration, p. 189 */
		case REQUEST(USB_RequestCode_GetConfiguration, ATTR_D2H_STD_DEV):
			handled = _USB_handle_get_configuration();
			break;

		/* USB 1.1 Specification, section 9.4.3 Get Descriptor, p. 189 */
		case REQUEST(USB_RequestCode_GetDescriptor, ATTR_D2H_STD_DEV):
		case REQUEST(USB_RequestCode_GetDescriptor, ATTR_D2H_STD_INT):
			handled = _USB_handle_get_descriptor(controlRequest->value, controlRequest->length);
			break;

		/* USB 1.1 Specification, section 9.4.5 Get Status, p. 190 */
		case REQUEST(USB_RequestCode_GetStatus, ATTR_D2H_STD_DEV):
			handled = _USB_handle_get_device_status();
			break;

		/* USB 1.1 Specification, section 9.4.5 Get Status, p. 190 */
		case REQUEST(USB_RequestCode_GetStatus, ATTR_D2H_STD_END):
			handled = _USB_handle_get_endpoint_status(controlRequest->index);
			break;

		/* USB 1.1 Specification, section 9.4.6 Set Address, p. 192 */
		case REQUEST(USB_RequestCode_SetAddress, ATTR_H2D_STD_DEV):
			handled = _USB_handle_set_address(controlRequest->value);
			break;

		// /* USB 1.1 Specification, section 9.4.7 Set Configuration, p. 193 */
		case REQUEST(USB_RequestCode_SetConfiguration, ATTR_H2D_STD_DEV):
			handled = _USB_handle_set_configuration(controlRequest->value);
			break;

		/* USB 1.1 Specification, section 9.4.9 Set Feature, p. 194 */
		case REQUEST(USB_RequestCode_SetFeature, ATTR_H2D_STD_DEV):
			handled = _USB_handle_set_device_feature(controlRequest->value);
			break;

		/* USB 1.1 Specification, section 9.4.9 Set Feature, p. 194 */
		case REQUEST(USB_RequestCode_SetFeature, ATTR_H2D_STD_END):
			handled = _USB_handle_set_endpoint_feature(controlRequest->value, controlRequest->index);
			break;

		/* USB HID 1.11 Specification, section 7.2.1 Get_Report Request, p. 51 */
		case REQUEST(HID_RequestCode_GetReport, ATTR_D2H_CLS_INT):
			handled = _USB_handle_get_report(controlRequest->length);
			break;

		/* USB HID 1.11 Specification, section 7.2.2 Set_Report Request, p. 52 */
		case REQUEST(HID_RequestCode_SetReport, ATTR_H2D_CLS_INT):
			handled = _USB_handle_set_report();
			break;

		/* USB HID 1.11 Specification, section 7.2.3 Get_Idle Request, p. 52 */
		case REQUEST(HID_RequestCode_GetIdle, ATTR_D2H_CLS_INT):
			handled = _USB_handle_get_idle();
			break;

		/* USB HID 1.11 Specification, section 7.2.4 Set_Idle Request, p. 52 */
		case REQUEST(HID_RequestCode_SetIdle, ATTR_H2D_CLS_INT):
			handled = _USB_handle_set_idle(controlRequest->value);
			break;

		/* USB HID 1.11 Specification, section 7.2.5 Get_Protocol Request, p. 54 */
		case REQUEST(HID_RequestCode_GetProtocol, ATTR_D2H_CLS_INT):
			handled = _USB_handle_get_protocol();
			break;

		/* USB HID 1.11 Specification, section 7.2.6 Set_Protocol Request, p. 54 */
		case REQUEST(HID_RequestCode_SetProtocol, ATTR_H2D_CLS_INT):
			handled = _USB_handle_set_protocol(controlRequest->value);
			break;

		default:
			break;
	}

	#undef ATTR_D2H_STD_DEV
	#undef ATTR_H2D_STD_DEV
	#undef ATTR_D2H_STD_END
	#undef ATTR_H2D_STD_END
	#undef ATTR_D2H_CLS_INT
	#undef ATTR_H2D_CLS_INT
	#undef REQUEST

	if (!handled)
	{
		USB_stall_endpoint();
	}
}

ISR(USB_COM_vect, ISR_BLOCK) {
	/*
		Make sure that no matter when this interrupt fires, it restores the global
		state of the USB hardware after it's finished.

		Even if it gets pre-empted by another interrupt that modifies the USB state
		during the period when interrupts are re-enabled, as long as that other
		interrupt also behaves itself, there should be no problems.
	*/

	u8 previousEndpoint = USB_get_selected_endpoint();
	USB_select_endpoint(USB_ENDPOINT_CONTROL);
	UEIENX &= ~_BV(RXSTPE);

	ENABLE_GLOBAL_INTERRUPTS();

	USB_DeviceRequest_t controlRequest;
	USB_read_from_endpoint(&controlRequest, sizeof(controlRequest));
	USB_clear_setup();
	_USB_handle_control_request(&controlRequest);

	USB_select_endpoint(USB_ENDPOINT_CONTROL);
	UEIENX |= _BV(RXSTPE);

	USB_select_endpoint(previousEndpoint);
}

#include "usb_reports.h"

local USB_NkroKeyboardReport_t previousReport;

bool USB_populate_boot_keyboard_report(USB_BootKeyboardReport_t *report) {
	USB_NkroKeyboardReport_t nkroReport = { 0 };
	bool hasUpdated = USB_populate_nkro_keyboard_report(&nkroReport);

	// Modifiers are 0xE0-0xE7, so they take up exactly one page of the NKRO report
	report->modifiers = nkroReport.keys[0xE0 / 8];

	for (u8 keyCodeIndex = 0, page = 0; page < 32 && keyCodeIndex < 6; ++page) {
		// Skip modifier key page
		if (page == 0xE0 / 8) {
			continue;
		}

		if (nkroReport.keys[page]) {
			for (u8 pageIndex = 0; pageIndex < 8 && keyCodeIndex < 6; ++pageIndex) {
				if (nkroReport.keys[page] & _BV(pageIndex)) {
					report->keyCodes[keyCodeIndex++] = (page * 8) + pageIndex;
				}
			}
		}
	}

	return hasUpdated;
}

local void _USB_set_nkro_key(USB_NkroKeyboardReport_t *report, u8 key) {
	report->keys[key / 8] |= _BV(key % 8);
}

// local u8 _USB_determine_keymap_layer() {
// 	u8 layer = 0;
// 	for (u8 row = 0; row < ROW_COUNT; ++row) {
// 		if (leftKeys.front[row] || rightKeys.front[row]) {
// 			for (u8 col = 0; col < COL_COUNT; ++col) {
// 				if (leftKeys.front[row] & _BV(col)) {
// 					u16 mapping = pgm_read_byte(&keymap.layers[layer].left[row][col]);
// 					if (mapping & KEYMAP_LAYER_SHIFT) {
// 						layer = mapping & KEYMAP_CODE_MASK;
// 					}
// 				}
// 				if (rightKeys.front[row] & _BV(col)) {
// 					u16 mapping = pgm_read_byte(&keymap.layers[layer].right[row][col]);
// 					if (mapping & KEYMAP_LAYER_SHIFT) {
// 						layer = mapping & KEYMAP_CODE_MASK;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	return layer;
// }

void USB_set_nkro_key_for_hex_value(USB_NkroKeyboardReport_t *report, u8 value) {
	if (value == 0x0) {
		_USB_set_nkro_key(report, HID_KEYPAD_0_INSERT);
	} else if (value < 0xA) {
		_USB_set_nkro_key(report, HID_KEYPAD_1_END + (value - 0x1));
	} else {
		_USB_set_nkro_key(report, HID_KEYBOARD_A + (value - 0xA));
	}
}

bool USB_populate_nkro_keyboard_report(USB_NkroKeyboardReport_t *report) {
	// u8 layer = _USB_determine_keymap_layer();

	// if (leftKeys.front[0]) _USB_set_nkro_key(report, HID_KEYBOARD_A);
	// if (leftKeys.front[1]) _USB_set_nkro_key(report, HID_KEYBOARD_B);
	// if (leftKeys.front[2]) _USB_set_nkro_key(report, HID_KEYBOARD_C);
	// if (leftKeys.front[3]) _USB_set_nkro_key(report, HID_KEYBOARD_D);
	// if (leftKeys.front[4]) _USB_set_nkro_key(report, HID_KEYBOARD_E);

	bool hasUpdated = memcmp(&previousReport, report, sizeof(previousReport)) != 0;
	memcpy(&previousReport, report, sizeof(previousReport));
	return hasUpdated;
}

void USB_process_led_report(USB_LedReport_t report) {
	u8 value = _BV(1);

	if (report & HID_LED_NUM_LOCK) {
		value |= _BV(0);
	}

	if (report & HID_LED_CAPS_LOCK) {
		value |= _BV(2);
	}

	if (report & HID_LED_SCROLL_LOCK) {
		value |= _BV(3);
	}

	// TODO: do something with this report
}

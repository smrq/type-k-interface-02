#include "keyboard.h"

local void _handle_press(USB_NkroKeyboardReport_t *keyboardReport, u16 *systemCode, u16 *consumerCode, u8 code, bool held) {
	if (KEYMAP_IS_SCANCODE(code)) {
		keyboardReport->keys[code / 8] |= _BV(code % 8);
	}

	else if (KEYMAP_IS_SYSTEM(code)) {
		*systemCode = keymap_code_to_system(code);
	}

	else if (KEYMAP_IS_CONSUMER(code)) {
		*consumerCode = keymap_code_to_consumer(code);
	}

	else if (KEYMAP_IS_USER(code)) {
		keymap_user_macro(keymap_code_to_user(code), held, keyboardReport, systemCode, consumerCode);
	}

	else if (KEYMAP_IS_FIRMWARE(code)) {
		switch (code) {
			case KC_FW_RESET:
				reset_to_bootloader();
				break;
		}
	}
}

local void _handle_matrix() {
	USB_NkroKeyboardReport_t keyboardReport = { 0 };
	u16 systemCode = 0;
	u16 consumerCode = 0;

	for (u8 column = 0; column < COLUMN_COUNT; ++column) {
		u8 columnState = keyscanner_get_state(column);
		if (columnState) {
			u8 previousState = keyscanner_get_previous_state(column);
			for (u8 row = 0; row < ROW_COUNT; ++row) {
				if (columnState & _BV(row)) {
					// TODO: layer shifts
					_handle_press(&keyboardReport, &systemCode, &consumerCode, pgm_read_byte(&keymap.layers[0][row][column]), previousState & _BV(row));
				}
			}
		}
	}

	switch (encoder_get_state()) {
		case Encoder_State_CW:
			_handle_press(&keyboardReport, &systemCode, &consumerCode, pgm_read_byte(&keymap.encoder.cw), false);
			break;

		case Encoder_State_CCW:
			_handle_press(&keyboardReport, &systemCode, &consumerCode, pgm_read_byte(&keymap.encoder.ccw), false);
			break;
	}

	USB_set_keyboard_report(&keyboardReport);
	USB_set_system_report(systemCode);
	USB_set_consumer_report(consumerCode);
}

void keyboard_update() {
	keyscanner_update();
	encoder_update();
	_handle_matrix();
}

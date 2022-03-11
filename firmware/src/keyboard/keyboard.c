#include "keyboard.h"

local void _handle_scancode(USB_NkroKeyboardReport_t *report, u8 scancode) {
	report->keys[scancode / 8] |= _BV(scancode % 8);
}

local void _handle_press(USB_NkroKeyboardReport_t *report, u8 code) {
	if (KEYMAP_IS_SCANCODE(code)) {
		_handle_scancode(report, code);
	}
	// TODO: consumer page
	// TODO: user macros
	// TODO: firmware keys
	// TODO: layer shifts
}

local void _handle_matrix() {
	USB_NkroKeyboardReport_t report = { 0 };

	for (u8 column = 0; column < COLUMN_COUNT; ++column) {
		u8 columnState = keyscanner_get_state(column);
		if (columnState) {
			for (u8 row = 0; row < ROW_COUNT; ++row) {
				if (columnState & _BV(row)) {
					_handle_press(&report, pgm_read_byte(&keymap.layers[0][row][column]));
				}
			}
		}
	}

	switch (encoder_get_state()) {
		case Encoder_State_CW:
			_handle_press(&report, pgm_read_byte(&keymap.encoder.cw));
			break;

		case Encoder_State_CCW:
			_handle_press(&report, pgm_read_byte(&keymap.encoder.ccw));
			break;
	}

	USB_set_keyboard_report(&report);
}

void keyboard_update() {
	keyscanner_update();
	encoder_update();
	_handle_matrix();
}

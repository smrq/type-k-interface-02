#include "keyboard.h"

local USB_LedReport_t _status;

void keyboard_update() {
	keyscanner_update();
	encoder_update();
}

local void _keyboard_handle_scancode(USB_NkroKeyboardReport_t *report, u8 scancode) {
	report->keys[scancode / 8] |= _BV(scancode % 8);
}

local void _keyboard_handle_code(USB_NkroKeyboardReport_t *report, u8 code) {
	if (KEYMAP_IS_SCANCODE(code)) {
		_keyboard_handle_scancode(report, code);
	}
	// TODO: consumer page
	// TODO: user macros
	// TODO: firmware keys
	// TODO: layer shifts
}

void keyboard_get_scancodes(USB_NkroKeyboardReport_t *report) {
	for (u8 column = 0; column < COLUMN_COUNT; ++column) {
		u8 columnState = keyscanner_get_state(column);
		if (columnState) {
			for (u8 row = 0; row < ROW_COUNT; ++row) {
				if (columnState & _BV(row)) {
					_keyboard_handle_code(report, pgm_read_byte(&keymap.layers[0][row][column]));
				}
			}
		}
	}

	enum Encoder_State_t encoderState = encoder_get_state();
	switch (encoderState) {
		case Encoder_State_CW:
			_keyboard_handle_code(report, pgm_read_byte(&keymap.encoder.cw));
			break;
		case Encoder_State_CCW:
			_keyboard_handle_code(report, pgm_read_byte(&keymap.encoder.ccw));
			break;
	}
}

void keyboard_set_status(USB_LedReport_t report) {
	_status = report;
}

USB_LedReport_t keyboard_get_status() {
	return _status;
}

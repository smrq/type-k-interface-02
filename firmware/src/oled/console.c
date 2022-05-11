#include "console.h"

local i16 _cursorX = 0;
local bool _newline = false;

void console_write(const char *text) {
	while (*text) {
		if (*text == '\n') {
			_cursorX = 0;
			_newline = true;
		} else if (*text == '\r') {
			_cursorX = 0;
		} else {
			if (_newline) {
				OLED_scroll_vertical(FONT_HEIGHT-1);
				_newline = false;
			}
			_cursorX = OLED_draw_char(_cursorX, OLED_HEIGHT - FONT_HEIGHT, *text, font_regular, font_offset_regular, true);
			if (_cursorX >= OLED_WIDTH) {
				_cursorX = 0;
				_newline = true;
			}
		}
		++text;
	}
	OLED_flip();
}

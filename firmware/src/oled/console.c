#include "console.h"

i16 cursorX;
bool newline = false;

void console_write(const char *text) {
	while (*text) {
		if (*text == '\n') {
			cursorX = 0;
			newline = true;
		} else if (*text == '\r') {
			cursorX = 0;
		} else {
			if (newline) {
				OLED_scroll_vertical(FONT_HEIGHT);
				newline = false;
			}
			cursorX = OLED_draw_char(cursorX, OLED_HEIGHT - FONT_HEIGHT, *text, font_regular, font_offset_regular, true);
		}
		++text;
	}
	OLED_flip();
}

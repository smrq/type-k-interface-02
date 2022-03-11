#include "oled_animation.h"

local void _draw_status_indicator(i16 x0, i16 y0, i16 x1, i16 y1, const char *text, bool status) {
	if (status) {
		OLED_draw_filled_rectangle(x0, y0, x1, y1, true);
		OLED_draw_text(x0+4, y0, text, font_regular, font_offset_regular, false);
	} else {
		OLED_draw_text(x0+4, y0, text, font_regular, font_offset_regular, true);
	}
}

enum AnimationResult_t animation_oled_keyboard_info(u16 frame) {
	// TODO: Indicator for boot keyboard mode
	OLED_clear();

	i16 x = 0;
	x = OLED_draw_text(x, -3, "TYPE-K ", font_regular, font_offset_regular, true);
	x = OLED_draw_text(x, -3, "$s?|U'|9", font_kana, font_offset_kana, true); // "インターフェース"
	x = OLED_draw_text(x, -3, " 02", font_regular, font_offset_regular, true);
	OLED_draw_horizontal_line(0, 127, 10, true);

	USB_LedReport_t status = USB_get_led_report();
	_draw_status_indicator(0, 11, 28, 24, "NUM", status & HID_LED_NUM_LOCK);
	_draw_status_indicator(47, 11, 81, 24, "CAPS", status & HID_LED_CAPS_LOCK);
	_draw_status_indicator(99, 11, 127, 24, "SCR", status & HID_LED_SCROLL_LOCK);

	OLED_flip();
	return AnimationResult_Continue;
}

#include "shared_animation.h"

enum AnimationResult_t animation_shared_startup(u16 frame) {
	switch (frame) {
		case 0:
			OLED_clear();
			OLED_draw_pixel(64, 16, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 8:
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 13:
			OLED_draw_filled_donut(64, 16, 18, 10, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 18:
			OLED_clear();
			OLED_draw_filled_donut(64, 16, 35, 30, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 22:
			OLED_clear();
			OLED_draw_filled_donut(64, 16, 40, 37, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 26:
			OLED_clear();
			OLED_draw_filled_donut(64, 16, 45, 43, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 29:
			OLED_clear();
			OLED_draw_filled_donut(64, 16, 47, 45, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 32:
			OLED_clear();
			OLED_draw_circle(64, 16, 49, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 35:
			OLED_clear();
			OLED_draw_circle(64, 16, 50, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 41:
			OLED_clear();
			OLED_draw_pixel(64, 16, true);
			OLED_flip();
			return AnimationResult_Continue;

		case 47:
			OLED_clear();
			OLED_flip();
			return AnimationResult_Continue;

		case 80:
			return AnimationResult_Finished;

		default:
			return AnimationResult_Continue;
	}
}

enum AnimationResult_t animation_shared_debug(u16 frame) {
	LED_clear();
	OLED_clear();

	OLED_draw_horizontal_line(0, 3*COLUMN_COUNT+2, 0, true);
	OLED_draw_horizontal_line(0, 3*COLUMN_COUNT+2, 3*ROW_COUNT+2, true);
	OLED_draw_vertical_line(0, 0, 3*ROW_COUNT+2, true);
	OLED_draw_vertical_line(3*COLUMN_COUNT+2, 0, 3*ROW_COUNT+2, true);

	for (u8 column = 0; column < COLUMN_COUNT; ++column) {
		u8 rowState = keyscanner_get_state(column);
		for (u8 row = 0; row < ROW_COUNT; ++row) {
			if (rowState & _BV(row)) {
				LED_set(LED_matrix_to_index(row, column), 0x0a0a0a);
				OLED_draw_pixel(2 + column*3, 2 + row*3, true);
				OLED_draw_pixel(3 + column*3, 2 + row*3, true);
				OLED_draw_pixel(2 + column*3, 3 + row*3, true);
				OLED_draw_pixel(3 + column*3, 3 + row*3, true);
			}
		}
	}

	enum Encoder_State_t encoderState = encoder_get_state();
	OLED_draw_text(3*COLUMN_COUNT+6, -2,
		encoderState == Encoder_State_CW ? "cw" :
		encoderState == Encoder_State_CCW ? "ccw" :
		"",
		font_regular, font_offset_regular, true);

	LED_update();
	OLED_flip();

	return AnimationResult_Continue;
}

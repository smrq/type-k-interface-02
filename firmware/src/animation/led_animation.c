#include "led_animation.h"

enum AnimationResult_t animation_led_startup(u16 frame) {
	if (frame == 0) {
		LED_clear();
		LED_update();
		return AnimationResult_Continue;
	} else if (frame < 50) {
		return AnimationResult_Continue;
	} else if (frame < 110) {
		u16 center_x = 192;
		u16 center_y = 50;

		u16 dMinInner = 0;
		u16 dMaxInner = 250;
		u16 dMinOuter = 40;
		u16 dMaxOuter = 250;
		u16 tMax = 150-50;

		i32 t = frame-50;

		u16 d_inner = dMaxInner - ((dMaxInner-dMinInner)*(t-tMax)*(t-tMax)/tMax/tMax);
		u16 d_outer = dMaxOuter - ((dMaxOuter-dMinOuter)*(t-tMax)*(t-tMax)/tMax/tMax);
		u16 d_peak = (d_outer + d_inner) / 2;

		u16 d2_inner = d_inner * d_inner;
		u16 d2_outer = d_outer * d_outer;
		u16 d2_peak = d_peak * d_peak;

		for (u8 n = 0; n < LED_COUNT; ++n) {
			u16 coords = LED_get_coordinates_xy(n);
			u16 x = (coords >> 8) & 0xFF;
			u16 y = coords & 0xFF;

			u16 dx = x > center_x ? x - center_x : center_x - x;
			u16 dy = y > center_y ? y - center_y : center_y - y;
			u16 dd2 = dx*dx + dy*dy;
		
			if (dd2 > d2_outer || dd2 < d2_inner) {
				LED_set_rgb(n, (color_rgb_t){ 0, 0, 0 });
			} else {
				color_hsv_t hsv = { 145, 235, 150 };
				if (dd2 > d2_peak) {
					hsv.v = (u32)hsv.v * (d2_outer - dd2) / (d2_outer - d2_peak);
				} else {
					hsv.v = (u32)hsv.v * (dd2 - d2_inner) / (d2_peak - d2_inner);
				}
				LED_set_hsv(n, hsv);
			}
		}

		LED_update();
		return AnimationResult_Continue;
	} else {
		return AnimationResult_Finished;
	}
}

enum AnimationResult_t animation_led_race(u16 frame) {
	u16 n = frame / 2;
	LED_clear();
	LED_set_rgb(n, (color_rgb_t){ 10, 10, 10 });
	LED_update();
	return ((frame+1)/2 == LED_COUNT) ?
		AnimationResult_Finished :
		AnimationResult_Continue;	
}

enum AnimationResult_t animation_led_gradient(u16 frame) {
	for (u8 n = 0; n < LED_COUNT; ++n) {
		color_hsv_t hsv = { 14, 255, 60 };
		u16 coords = LED_get_coordinates_xy(n);
		u16 x = (coords >> 8) & 0xFF;
		i16 sine = pgm_read_byte(&math_sine_table[(256 + (x/2) - (frame/2)) % 256]) - 0x80;
		hsv.h += sine * 15 / 255;
		LED_set_hsv(n, hsv);
	}
	LED_update();
	return AnimationResult_Continue;
}

enum AnimationResult_t animation_led_debug(u16 frame) {
	LED_clear();

	for (u8 column = 0; column < COLUMN_COUNT; ++column) {
		u8 rowState = keyscanner_get_state(column);
		for (u8 row = 0; row < ROW_COUNT; ++row) {
			if (rowState & _BV(row)) {
				LED_set_rgb(LED_matrix_to_index(row, column), (color_rgb_t){ 10, 10, 10 });
			}
		}
	}

	LED_update();

	return AnimationResult_Continue;
}

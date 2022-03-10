#include "animation.h"

enum AnimationState_t {
	AnimationState_Startup,
	AnimationState_KeyboardInfo
};

local enum AnimationState_t _animationState;
local u32 _animationLastFrameTime;
local u16 _animationFrame;

void animation_init() {
	_animationState = AnimationState_Startup;
	_animationFrame = 0;
	_animationLastFrameTime = timer_get_ms();
}

local bool _animation_startup() {
	switch (_animationFrame++) {
		case 0:
			OLED_clear();
			OLED_draw_pixel(64, 16, true);
			OLED_flip();
			return false;

		case 8:
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return false;

		case 13:
			OLED_draw_filled_donut(64, 16, 18, 10, true);
			OLED_flip();
			return false;

		case 18:
			OLED_clear();
			OLED_draw_filled_donut(64, 16, 35, 30, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return false;

		case 22:
			OLED_clear();
			OLED_draw_filled_donut(64, 16, 40, 37, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return false;

		case 26:
			OLED_clear();
			OLED_draw_filled_donut(64, 16, 45, 43, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return false;

		case 29:
			OLED_clear();
			OLED_draw_filled_donut(64, 16, 47, 45, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return false;

		case 32:
			OLED_clear();
			OLED_draw_circle(64, 16, 49, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return false;

		case 35:
			OLED_clear();
			OLED_draw_circle(64, 16, 50, true);
			OLED_draw_filled_circle(64, 16, 1, true);
			OLED_flip();
			return false;

		case 41:
			OLED_clear();
			OLED_draw_pixel(64, 16, true);
			OLED_flip();
			return false;

		case 47:
			OLED_clear();
			OLED_flip();
			return false;

		case 60:
			return true;

		default:
			return false;
	}
}

local void _animation_keyboard_info() {
	OLED_clear();
	const char str1[] = "TYPE-K ";
	const char str2[] = "$s?|U'|9"; // "インターフェース"
	const char str3[] = " 02";
	i16 x = 0;
	x = OLED_draw_text(x, -2, str1, font_regular, font_offset_regular, true);
	x = OLED_draw_text(x, -2, str2, font_kana, font_offset_kana, true);
	x = OLED_draw_text(x, -2, str3, font_regular, font_offset_regular, true);
	OLED_draw_horizontal_line(0, 127, 11, true);
	OLED_flip();
}

void animation_tick() {
	u32 t = timer_get_ms();
	if (t - _animationLastFrameTime > ANIMATION_MS_PER_FRAME) {
		_animationLastFrameTime = t;
		switch (_animationState) {
			case AnimationState_Startup:
				if (_animation_startup()) {
					_animationState = AnimationState_KeyboardInfo;
					_animationFrame = 0;
				}
				break;

			case AnimationState_KeyboardInfo:
				_animation_keyboard_info();
				break;
		}
	}
}

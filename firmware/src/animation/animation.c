#include "animation.h"

local AnimationState_OLED_t _oledState;
local AnimationState_LED_t _ledState;
local u32 _lastFrameTime;

void animation_init() {
	_oledState = (AnimationState_OLED_t){ .animation = Animation_OLED_Startup };
	_ledState = (AnimationState_LED_t){ .animation = Animation_LED_Startup };
	_lastFrameTime = timer_ms;
}

local void _oled_frame() {
	enum AnimationResult_t result;

	switch (_oledState.animation) {
		case Animation_OLED_Startup:
			result = animation_oled_startup(_oledState.frame);
			break;
		case Animation_OLED_KeyboardInfo:
			result = animation_oled_keyboard_info(_oledState.frame);
			break;
		case Animation_OLED_Debug:
			result = animation_oled_debug(_oledState.frame);
			break;
	}

	++_oledState.frame;

	if (result == AnimationResult_Finished) {
		switch (_oledState.animation) {
			case Animation_OLED_Startup:
				_oledState = (AnimationState_OLED_t){ .animation = Animation_OLED_KeyboardInfo };
				break;
			default:
				_oledState.frame = 0;
				break;
		}
	}
}

local void _led_frame() {
	enum AnimationResult_t result;

	switch (_ledState.animation) {
		case Animation_LED_Startup:
			result = animation_led_startup(_ledState.frame);
			break;
		case Animation_LED_Race:
			result = animation_led_race(_ledState.frame);
			break;
		case Animation_LED_Gradient:
			result = animation_led_gradient(_ledState.frame);
			break;
		case Animation_LED_Debug:
			result = animation_led_debug(_ledState.frame);
			break;
	}

	++_ledState.frame;

	if (result == AnimationResult_Finished) {
		switch (_ledState.animation) {
			case Animation_LED_Startup:
				_ledState = (AnimationState_LED_t){ .animation = Animation_LED_Gradient };
				break;
			default:
				_ledState.frame = 0;
				break;
		}
	}
}

void animation_tick() {
	u32 t = timer_ms;
	if (t - _lastFrameTime > ANIMATION_MS_PER_FRAME) {
		_lastFrameTime = t;
		_oled_frame();
		_led_frame();
	}
}

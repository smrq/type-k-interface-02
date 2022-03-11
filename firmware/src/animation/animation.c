#include "animation.h"

local AnimationState_t _state;
local u32 _lastFrameTime;

void animation_init() {
	_state = (AnimationState_t){
		.type = AnimationType_Shared,
		.data = { .shared = { .animation = Animation_Shared_Startup }}
	};
	_lastFrameTime = timer_get_ms();
}

local void _shared_frame() {
	enum AnimationResult_t result;

	switch (_state.data.shared.animation) {
		case Animation_Shared_Startup:
			result = animation_shared_startup(_state.data.shared.frame++);
			break;
		case Animation_Shared_Debug:
			result = animation_shared_debug(_state.data.shared.frame++);
			break;
	}

	if (result == AnimationResult_Finished) {
		switch (_state.data.shared.animation) {
			case Animation_Shared_Startup:
				_state = (AnimationState_t){
					.type = AnimationType_Split,
					.data = { .split = {
						.oled = { .animation = Animation_OLED_KeyboardInfo },
						.led = { .animation = Animation_LED_Race }
					}}
				};
				break;
			default:
				_state.data.shared.frame = 0;
				break;
		}
	}
}

local void _split_frame() {
	enum AnimationResult_t oledResult;
	enum AnimationResult_t ledResult;

	switch (_state.data.split.oled.animation) {
		case Animation_OLED_KeyboardInfo:
			oledResult = animation_oled_keyboard_info(_state.data.split.oled.frame++);
			break;
	}

	switch (_state.data.split.led.animation) {
		case Animation_LED_Race:
			ledResult = animation_led_race(_state.data.split.led.frame++);
			break;
	}

	if (oledResult == AnimationResult_Finished) {
		switch (_state.data.split.oled.animation) {
			default:
				_state.data.split.oled.frame = 0;
				break;
		}
	}

	if (ledResult == AnimationResult_Finished) {
		switch (_state.data.split.led.animation) {
			default:
				_state.data.split.led.frame = 0;
				break;
		}
	}
}

void animation_tick() {
	u32 t = timer_get_ms();
	if (t - _lastFrameTime > ANIMATION_MS_PER_FRAME) {
		_lastFrameTime = t;
		if (_state.type == AnimationType_Shared) {
			_shared_frame();
		} else {
			_split_frame();
		}
	}
}
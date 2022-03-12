#pragma once

#include "../defs.h"
#include "../timer.h"

#define ANIMATION_MS_PER_FRAME 16

enum Animation_OLED_t {
	Animation_OLED_None,
	Animation_OLED_Startup,
	Animation_OLED_KeyboardInfo,
	Animation_OLED_Debug,
};

enum Animation_LED_t {
	Animation_LED_None,
	Animation_LED_Startup,
	Animation_LED_Race,
	Animation_LED_Gradient,
	Animation_LED_Debug,
};

enum AnimationResult_t {
	AnimationResult_Continue,
	AnimationResult_Finished,
};

typedef struct {
	enum Animation_OLED_t animation;
	u16 frame;
} AnimationState_OLED_t;

typedef struct {
	enum Animation_LED_t animation;
	u16 frame;
} AnimationState_LED_t;

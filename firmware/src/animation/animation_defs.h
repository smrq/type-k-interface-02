#pragma once

#include "../defs.h"
#include "../timer.h"

#define ANIMATION_MS_PER_FRAME 16

enum AnimationType_t {
	AnimationType_Shared,
	AnimationType_Split
};

enum Animation_Shared_t {
	Animation_Shared_None,
	Animation_Shared_Startup,
	Animation_Shared_Debug
};

enum Animation_OLED_t {
	Animation_OLED_None,
	Animation_OLED_KeyboardInfo
};

enum Animation_LED_t {
	Animation_LED_None,
	Animation_LED_Race
};

enum AnimationResult_t {
	AnimationResult_Continue,
	AnimationResult_Finished
};

typedef struct {
	enum Animation_Shared_t animation;
	u16 frame;
} AnimationState_Shared_t;

typedef struct {
	enum Animation_OLED_t animation;
	u16 frame;
} AnimationState_OLED_t;

typedef struct {
	enum Animation_LED_t animation;
	u16 frame;
} AnimationState_LED_t;

typedef struct {
	enum AnimationType_t type;
	union {
		AnimationState_Shared_t shared;
		struct {
			AnimationState_OLED_t oled;
			AnimationState_LED_t led;
		} split;
	} data;
} AnimationState_t;

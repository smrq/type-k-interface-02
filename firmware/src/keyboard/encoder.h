#pragma once

#include "../defs.h"

enum Encoder_State_t {
	Encoder_State_None,
	Encoder_State_CW,
	Encoder_State_CCW
};

void encoder_update();
enum Encoder_State_t encoder_get_state();

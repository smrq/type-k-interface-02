#pragma once

#include "../defs.h"

typedef struct {
	u8 r;
	u8 g;
	u8 b;
} color_rgb_t;

typedef struct {
	u8 h;
	u8 s;
	u8 v;
} color_hsv_t;

color_rgb_t color_hsv_to_rgb(color_hsv_t hsv);

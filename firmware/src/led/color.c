#include "color.h"

color_rgb_t color_hsv_to_rgb(color_hsv_t hsv) {
	if (hsv.s == 0) {
		return (color_rgb_t){ hsv.v, hsv.v, hsv.v };
	}

	u16 h = hsv.h;
	u16 s = hsv.s;
	u16 v = hsv.v;

	u8 region = h*6 / 255;
	u8 remainder = (h*2 - region*85)*3;

	u8 p = (v * (255 - s)) >> 8;
	u8 q = (v * (255 - ((s * remainder) >> 8))) >> 8;
	u8 t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

	switch (region) {
		case 6:
		case 0:
			return (color_rgb_t){ v, t, p };
		case 1:
			return (color_rgb_t){ q, v, p };
		case 2:
			return (color_rgb_t){ p, v, t };
		case 3:
			return (color_rgb_t){ p, q, v };
		case 4:
			return (color_rgb_t){ t, p, v };
		default:
			return (color_rgb_t){ v, p, q };
	}
}

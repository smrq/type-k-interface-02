#pragma once

#include "../defs.h"
#include "color.h"

#define LED_COUNT 128
#define LED_MAX_X 0xFF
#define LED_MAX_Y 0x4D

void LED_init();
void LED_clear();
void LED_set_rgb(u8 n, color_rgb_t color);
void LED_set_hsv(u8 n, color_hsv_t color);
u16 LED_get_coordinates_xy(u8 n);
void LED_update();
u8 LED_matrix_to_index(u8 row, u8 column);

#pragma once

#include "../defs.h"

#define LED_COUNT 128

void LED_init();
void LED_clear();
void LED_set(u8 n, u32 rgb);
void LED_set_parametric(u32 (*fn)(u8, u8));
void LED_update();
u8 LED_matrix_to_index(u8 row, u8 column);

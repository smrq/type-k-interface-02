#pragma once

#include "../defs.h"

#define LED_COUNT 128

void LED_init();
void LED_set(u8 n, u8 r, u8 g, u8 b);
void LED_update();

#pragma once

#include "defs.h"
#include "twi.h"

#include "led.h" // for testing

void OLED_init();
void OLED_flip();
void OLED_clear();
void OLED_test_pattern(u8 n);

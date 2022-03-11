#pragma once

#include "defs.h"
#include "timer.h"
#include "keyboard/keyboard.h"
#include "keyboard/keyscanner.h"
#include "keyboard/encoder.h"
#include "led/led.h"
#include "oled/oled.h"
#include "usb/usb_defs.h"

#define ANIMATION_MS_PER_FRAME 16

void animation_init();
void animation_tick();

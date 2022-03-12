#pragma once

#include "../defs.h"
#include "../math.h"
#include "../led/color.h"
#include "../led/led.h"

#include "../keyboard/keyboard.h"
#include "../keyboard/keyscanner.h"
#include "../keyboard/encoder.h"
#include "../usb/usb_defs.h"
#include "../usb/usb_reports.h"

#include "animation_defs.h"

enum AnimationResult_t animation_led_startup(u16 frame);
enum AnimationResult_t animation_led_race(u16 frame);
enum AnimationResult_t animation_led_gradient(u16 frame);
enum AnimationResult_t animation_led_debug(u16 frame);

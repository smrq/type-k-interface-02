#pragma once

#include "../defs.h"
#include "../fonts/fonts.h"
#include "../led/led.h"
#include "../oled/oled.h"

#include "../keyboard/keyboard.h"
#include "../keyboard/keyscanner.h"
#include "../keyboard/encoder.h"
#include "../usb/usb_defs.h"
#include "../usb/usb_reports.h"

#include "animation_defs.h"

enum AnimationResult_t animation_shared_startup(u16 frame);
enum AnimationResult_t animation_shared_debug(u16 frame);

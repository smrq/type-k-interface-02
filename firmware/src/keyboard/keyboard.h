#pragma once

#include "../defs.h"
#include "../keymap.h"
#include "../usb/usb_defs.h"
#include "keyscanner.h"
#include "encoder.h"

void keyboard_update();
void keyboard_get_scancodes(USB_NkroKeyboardReport_t *report);
void keyboard_set_status(USB_LedReport_t status);
u8 keyboard_get_status();

#pragma once

#include "../defs.h"
#include "../keyboard/keyboard.h"
#include "usb_defs.h"

bool USB_populate_boot_keyboard_report(USB_BootKeyboardReport_t *report);
bool USB_populate_nkro_keyboard_report(USB_NkroKeyboardReport_t *report);
void USB_process_led_report(USB_LedReport_t report);

#pragma once

#include "../defs.h"
#include "usb_defs.h"

extern bool USB_using_report_protocol;

void USB_set_keyboard_report(USB_NkroKeyboardReport_t *report);
bool USB_is_keyboard_report_dirty();
void USB_get_keyboard_report(void **report, u16 *length);
void USB_set_led_report(USB_LedReport_t report);
USB_LedReport_t USB_get_led_report();

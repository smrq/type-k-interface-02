#pragma once

#include "../defs.h"
#include "usb_defs.h"

typedef struct {
	u8 keys[32];
} PACKED USB_NkroKeyboardReport_t;

typedef struct {
	u8 modifiers;
	u8 reserved;
	u8 keyCodes[6];
} PACKED USB_BootKeyboardReport_t;

typedef u8 USB_LedReport_t;

bool USB_populate_boot_keyboard_report(USB_BootKeyboardReport_t *report);
bool USB_populate_nkro_keyboard_report(USB_NkroKeyboardReport_t *report);
void USB_process_led_report(USB_LedReport_t report);

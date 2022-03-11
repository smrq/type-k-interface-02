#pragma once

#include "../defs.h"
#include "usb_defs.h"
#include "usb_hardware.h"
#include "usb_reports.h"

extern u8 USB_configuration_number;
extern bool USB_remote_wakeup_enabled;
extern u16 USB_idle_timeout_duration;
extern u16 USB_idle_timeout_remaining;

void USB_init();
void USB_update();
void USB_send_system(u16 code);
void USB_send_consumer(u16 code);

#pragma once

#include "../defs.h"
#include "usb_defs.h"
#include "usb_hardware.h"
#include "usb_reports.h"

extern u8 USB_configuration_number;
extern bool USB_remote_wakeup_enabled;
extern bool USB_using_report_protocol;
extern u16 USB_idle_timeout_duration;
extern u16 USB_idle_timeout_remaining;

void USB_init();
USB_LedReport_t USB_update();

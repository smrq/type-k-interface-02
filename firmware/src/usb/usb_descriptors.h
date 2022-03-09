#pragma once

#include "../defs.h"
#include "usb_defs.h"
#include "usb_hardware.h"

bool USB_get_descriptor(u8 type, u8 number, const void **address, u16 *size);

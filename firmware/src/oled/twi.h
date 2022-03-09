#pragma once

#include "../defs.h"

#define TWI_BLOCKING(CMD) do { CMD; while (TWI_busy()); } while (0)

void TWI_init();
bool TWI_busy();
bool TWI_send(u8 address, const u8 *txData, size_t txLength);
bool TWI_send_progmem(u8 address, const u8 *txData, size_t txLength);
// bool TWI_send_8(u8 address, u8 txData);
// bool TWI_send_16(u8 address, u8 txData0, u8 txData1);
// bool TWI_send_24(u8 address, u8 txData0, u8 txData1, u8 txData2);
// bool TWI_send_32(u8 address, u8 txData0, u8 txData1, u8 txData2, u8 txData3);
// bool TWI_receive(u8 address, u8 *rxData, size_t rxLength);
// bool TWI_send_receive(u8 address, const u8 *txData, size_t txLength, u8 *rxData, size_t rxLength);


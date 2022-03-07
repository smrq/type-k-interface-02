#include "defs.h"
#include "twi.h"

enum MessageState_t {
	STATE_IDLE,
	STATE_TX,
	STATE_TX_PROGMEM,
	STATE_FINISHED
};

local u8 messageAddress = 0;
local const u8 *messageTxBuffer;
local size_t messageTxLength;
local volatile enum MessageState_t messageState = STATE_IDLE;

void TWI_init() {
	/*
		TWBR should be 10 or higher if the TWI operates in Master mode. If TWBR is lower than 10,
		the Master may produce an incorrect output on SDA and SCL for the reminder of the byte.
		The problem occurs when operating the TWI in Master mode, sending Start + SLA + R/W to a
		Slave (a Slave does not need to be connected to the bus for the condition to happen).
			- ATmega32U4 datasheet, p. 231
	*/
	TWBR = 10;

	// Set bit rate prescaler to 1
	TWSR &= ~(_BV(TWPS0) | _BV(TWPS1));
}

local void TWI_start() {
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) |  _BV(TWSTA);
}

local void TWI_continue(bool ack) {
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | (ack ? _BV(TWEA) : 0);
}

local void TWI_stop() {
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWSTO);
	messageState = STATE_IDLE;
}

bool TWI_busy() {
	return messageState != STATE_IDLE || (TWCR & _BV(TWSTO));
}

bool TWI_send(u8 address, const u8 *txData, size_t txLength) {
	if (TWI_busy()) {
		return false;
	}
	
	messageAddress = address;
	messageTxBuffer = txData;
	messageTxLength = txLength;
	messageState = STATE_TX;
	TWI_start();

	return true;
}

bool TWI_send_progmem(u8 address, const u8 *txData, size_t txLength) {
	if (TWI_busy()) {
		return false;
	}

	messageAddress = address;
	messageTxBuffer = txData;
	messageTxLength = txLength;
	messageState = STATE_TX_PROGMEM;
	TWI_start();

	return true;
}

ISR(TWI_vect) {
	switch (TW_STATUS) {
		case TW_START: // 0x08
		case TW_REP_START: // 0x10
			switch (messageState) {
				case STATE_TX:
				case STATE_TX_PROGMEM:
					TWDR = (messageAddress << 1) | TW_WRITE;
					TWI_continue(true); // SLA+W will be transmitted; ACK or NOT ACK will be received
					break;

				default: // This should never happen.
					TWDR = 0xFE | TW_WRITE; // Nobody should respond to this.
					TWI_continue(true); // SLA+W will be transmitted; ACK or NOT ACK will be received
					break;
			}
			break;

		// Master transmit mode
		case TW_MT_SLA_ACK: // 0x18
		case TW_MT_DATA_ACK: // 0x28
			switch (messageState) {
				case STATE_TX:
					TWDR = *messageTxBuffer++;
					if (!--messageTxLength) {
						messageState = STATE_FINISHED;
					}
					TWI_continue(true); // Data byte will be transmitted and ACK or NOT ACK will be received
					break;

				case STATE_TX_PROGMEM:
					TWDR = pgm_read_byte(messageTxBuffer++);
					if (!--messageTxLength) {
						messageState = STATE_FINISHED;
					}
					TWI_continue(true); // Data byte will be transmitted and ACK or NOT ACK will be received
					break;				

				case STATE_FINISHED:
					TWI_stop(); // STOP condition will be transmitted and TWSTO Flag will be reset
					break;

				default: // This should never happen.
					TWI_stop(); // STOP condition will be transmitted and TWSTO Flag will be reset
					break;
			}
			break;

		// Master receive mode
		case TW_MR_DATA_ACK: // 0x50
		case TW_MR_DATA_NACK: // 0x58
		
		// Error cases
		case TW_MT_SLA_NACK: // 0x20
		case TW_MT_DATA_NACK: // 0x28
		case TW_MR_SLA_NACK: // 0x48
			TWI_stop(); // STOP condition will be transmitted and TWSTO Flag will be reset
			break;

		case TW_MT_ARB_LOST: // and TW_MR_ARB_LOST: 0x38  This should never happen because we are the only master
			TWI_start(); // A START condition will be transmitted when the bus becomes free
			break;

		case TW_BUS_ERROR: // 0x00
		default:
			// Only the internal hardware is affected, no STOP condition is sent on the bus.
			// In all cases, the bus is released and TWSTO is cleared.
			TWI_stop();
			break;
	}
}

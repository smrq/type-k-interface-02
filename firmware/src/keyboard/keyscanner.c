#include "keyscanner.h"

local Keyscanner_ColumnState_t _columnState[COLUMN_COUNT] = { 0 };

local void keyscanner_debounce(Keyscanner_ColumnState_t *column, u8 pressed) {
	/*
		Two-bit counter

		Once a key state has changed for 4 consecutive frames, this counter
		will overflow and we should register the key as updated.

		If a key state is the same as what is currently registered, then
		the counter for that key should be reset.

		   Input   00   01   10   11
		OVERFLOW    0    0    0    1  = LO & HI
		    HIGH    0    1    1    0  = LO ^ HI
		     LOW    1    0    1    0  = ~LO
	*/
	u8 delta = column->current ^ pressed;
	u8 counterOverflow = delta & column->counter1 & column->counter0;
	column->counter1 = delta & (column->counter1 ^ column->counter0);
	column->counter0 = delta & ~(column->counter0);
	column->current ^= counterOverflow;
}

void keyscanner_update() {
	// Column bit pins (MSB to LSB): D7 E6 B4 B5
	for (u8 column = 0; column < COLUMN_COUNT; ++column) {
		PORTB = (PORTB & ~(_BV(4) | _BV(5))) | (((column >> 0) & 1) << 5) | (((column >> 1) & 1) << 4);
		PORTE = (PORTE & ~(_BV(6))) | (((column >> 2) & 1) << 6);
		PORTD = (PORTD & ~(_BV(7))) | (((column >> 3) & 1) << 7);

		// Wait for signal to propagate
		_delay_us(1);

		// Row pins (MSB to LSB): F7 F6 F5 F4 B2 B3 B1 D3
		u8 rows = ~(
			(((PIND >> 3) & 1) << 0) |
			(((PINB >> 1) & 1) << 1) |
			(((PINB >> 3) & 1) << 2) |
			(((PINB >> 2) & 1) << 3) |
			(PINF & 0xF0)
		);

		_columnState[column].previous = _columnState[column].current;
		keyscanner_debounce(&_columnState[column], rows);
	}
	PORTB = PORTB & ~(_BV(4) | _BV(5));
	PORTE = PORTE & ~(_BV(6));
	PORTD = PORTD & ~(_BV(7));
}

u8 keyscanner_get_state(u8 column) {
	return _columnState[column].current;
}

u8 keyscanner_get_previous_state(u8 column) {
	return _columnState[column].previous;
}

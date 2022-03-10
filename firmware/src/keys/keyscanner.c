#include "keyscanner.h"

local Keyscanner_ColumnState_t columnState[COLUMN_COUNT] = { 0 };

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
	for (u8 column = 0; column < COLUMN_COUNT; ++column) {
		PORTB = (PORTB & ~(_BV(4) | _BV(5))) | ((column & 1) << 5) | (((column >> 1) & 1) << 4);
		PORTD = (PORTD & ~(_BV(7))) | (((column >> 3) & 1) << 7);
		PORTE = (PORTE & ~(_BV(6))) | (((column >> 2) & 1) << 6);
		/*
			When reading back a software assigned pin value, a nop instruction must be
			inserted as indicated in Figure 10-4.
				- ATmega32U4 datasheet, p. 70

			Since we're using multiple ports, this should take more than a single instruction
			between each write and read.
		*/
		u8 pinb = PINB;
		u8 pind = PIND;
		u8 pinf = PINF;
		u8 rows = ~(
			((pind & _BV(3)) >> 3) |
			(pinb & _BV(1)) |
			((pinb & _BV(3)) >> 1) |
			((pinb & _BV(2)) << 1) |
			(pinf & (_BV(4) | _BV(5) | _BV(6) | _BV(7)))
		);

		keyscanner_debounce(&columnState[column], rows);
	}
	PORTB = PORTB & ~(_BV(4) | _BV(5));
	PORTE = PORTE & ~(_BV(6));
	PORTD = PORTD & ~(_BV(7));
}

u8 keyscanner_get_state(u8 column) {
	return columnState[column].current;
}

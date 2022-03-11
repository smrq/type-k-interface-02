#include "timer.h"

local volatile u32 _timer_ms = 0;

void timer_init() {
	// 13.8.1 Timer/Counter Control Register A – TCCR0A
	TCCR0A = _BV(WGM01);            // CTC mode
	TCCR0B = _BV(CS00) | _BV(CS01); // 1/64 clock prescaler (250kHz)
	OCR0A = 250;                    // 250 ticks per interrupt (1kHz = 1ms)
	TIMSK0 = _BV(OCIE0A);           // Enable interrupt
}

u32 timer_get_ms() {
	return _timer_ms;
}

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK) {
	++_timer_ms;
}

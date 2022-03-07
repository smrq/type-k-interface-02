#include "led.h"

void LED_init() {
	u8 data[3] = { 0 };
	LED_set_colors(data, sizeof(data));
}

void LED_set_colors(u8 *data, size_t length) {
	DISABLE_GLOBAL_INTERRUPTS();
	u8 hi = PORTD | _BV(4);
	u8 lo = PORTD & ~_BV(4);
	u8 index;
	while (length--) {
		u8 value = (*data++);
		// 0: 6 cycles high, 15 cycles low
		// 1: 15 cycles high, 6 cycles low
		asm volatile(
			"  ldi %[index], 8"     "\n\t"
			"loop%=:"               "\n\t"
			"  out %[portd], %[hi]" "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  sbrs %[value], 7"    "\n\t"
			"  out %[portd], %[lo]" "\n\t"
			"  lsl %[value]"        "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  out %[portd], %[lo]" "\n\t"
			"  nop"                 "\n\t"
			"  nop"                 "\n\t"
			"  dec %[index]"        "\n\t"
			"  brne loop%="         "\n\t"
			: [index] "=&d" (index)
			: [value] "r" (value),
			  [portd] "I" (_SFR_IO_ADDR(PORTD)),
			  [hi] "r" (hi),
			  [lo] "r" (lo)
		);
	}
	ENABLE_GLOBAL_INTERRUPTS();
	_delay_us(280);
}

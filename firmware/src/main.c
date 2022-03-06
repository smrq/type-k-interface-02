#include "main.h"

void send_led_data(u8 *data, size_t length) {
	disableGlobalInterrupts();
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
	enableGlobalInterrupts();
	_delay_us(280);
}

void setup() {
	CLKPR = _BV(CLKPCE);
	CLKPR = 0;

	// Disable watchdog timer
	// This should be the default, but after flashing with dfu-programmer,
	// the watchdog seems to be left running.
	MCUSR &= ~(1<<WDRF);
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = 0x00;

	DDRD |= _BV(3) | _BV(4);
	PORTD &= ~(_BV(3) | _BV(4));

	enableGlobalInterrupts();
}

u8 led_data[] = { 0x55, 0xAA, 0x00 };

void loop() {
	send_led_data(led_data, sizeof(led_data));
	++led_data[0];
	++led_data[1];
	++led_data[2];
	_delay_ms(16);
}

int main() {
	setup();

	while(1) {
		loop();
	}

	__builtin_unreachable();
}

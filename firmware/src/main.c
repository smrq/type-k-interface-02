#include "main.h"

void setup() {
	CLKPR = _BV(CLKPCE);
	CLKPR = 0;

	// Disable watchdog timer
	// This should be the default, but after flashing with dfu-programmer,
	// the watchdog seems to be left running.
	MCUSR &= ~(1<<WDRF);
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = 0x00;

	DDRD |= _BV(4);
	PORTD &= ~_BV(4);

	LED_init();
	TWI_init();
	OLED_init();

	ENABLE_GLOBAL_INTERRUPTS();
}

void loop() {
	LED_set_colors((u8[]){ 0, 10, 0 }, 3);
	OLED_test_pattern(0);
	OLED_flip();
	_delay_ms(250);

	LED_set_colors((u8[]){ 10, 0, 0 }, 3);
	OLED_test_pattern(1);
	OLED_flip();
	_delay_ms(250);

	LED_set_colors((u8[]){ 0, 0, 10 }, 3);
	OLED_test_pattern(2);
	OLED_flip();
	_delay_ms(250);
}

int main() {
	setup();

	while(1) {
		loop();
	}

	__builtin_unreachable();
}

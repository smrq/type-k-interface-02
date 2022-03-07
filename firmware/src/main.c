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
	OLED_clear();
	OLED_draw_pixel(64, 16, 1);
	OLED_flip();
	_delay_ms(100);

	OLED_draw_filled_circle(64, 16, 1, 1);
	OLED_flip();
	_delay_ms(100);

	OLED_draw_filled_circle(64, 16, 18, 1);
	OLED_draw_filled_circle(64, 16, 10, 0);
	OLED_draw_filled_circle(64, 16, 1, 1);
	OLED_flip();
	_delay_ms(60);

	OLED_draw_filled_circle(64, 16, 35, 1);
	OLED_draw_filled_circle(64, 16, 30, 0);
	OLED_draw_filled_circle(64, 16, 1, 1);
	OLED_flip();
	_delay_ms(60);

	OLED_draw_filled_circle(64, 16, 40, 1);
	OLED_draw_filled_circle(64, 16, 37, 0);
	OLED_draw_filled_circle(64, 16, 1, 1);
	OLED_flip();
	_delay_ms(40);

	OLED_draw_filled_circle(64, 16, 45, 1);
	OLED_draw_filled_circle(64, 16, 43, 0);
	OLED_draw_filled_circle(64, 16, 1, 1);
	OLED_flip();
	_delay_ms(40);

	OLED_draw_filled_circle(64, 16, 47, 1);
	OLED_draw_filled_circle(64, 16, 45, 0);
	OLED_draw_filled_circle(64, 16, 1, 1);
	OLED_flip();
	_delay_ms(40);

	OLED_clear();
	OLED_draw_circle(64, 16, 49, 1);
	OLED_draw_filled_circle(64, 16, 1, 1);
	OLED_flip();
	_delay_ms(40);

	OLED_clear();
	OLED_draw_circle(64, 16, 50, 1);
	OLED_draw_filled_circle(64, 16, 1, 1);
	OLED_flip();
	_delay_ms(100);

	OLED_clear();
	OLED_draw_pixel(64, 16, 1);
	OLED_flip();
	_delay_ms(100);

	OLED_clear();
	OLED_flip();
	_delay_ms(500);

}

int main() {
	setup();

	while(1) {
		loop();
	}

	__builtin_unreachable();
}

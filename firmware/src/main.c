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

	// Port B
	// 0: Unused       [input, pull-up]
	// 1: Row 1        [input, pull-up]
	// 2: Row 3        [input, pull-up]
	// 3: Row 2        [input, pull-up]
	// 4: Column bit 1 [output, normally low]
	// 5: Column bit 0 [output, normally low]
	// 6-7: Unused     [input, pull-up]
	PORTB = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(6) | _BV(7);
	DDRB = _BV(4) | _BV(5);

	// Port C
	// 6: Encoder A    [input, pull-up]
	// 7: Unused       [input, pull-up]
	PORTC = _BV(6) | _BV(7);
	DDRC = 0;

	// Port D
	// 0: SCL (ignored when TWEN bit in TWCR is set; external pull-up)
	// 1: SDA (ignored when TWEN bit in TWCR is set; external pull-up)
	// 2: Encoder B    [input, pull-up]
	// 3: Row 0        [input, pull-up]
	// 4: LED signal   [output, normally low]
	// 5-6: Unused     [input, pull-up]
	// 7: Column bit 3 [output, normally low]
	PORTD = _BV(2) | _BV(3) | _BV(5) | _BV(6);
	DDRD = _BV(4) | _BV(7);

	// Port E
	// 2: Unused       [input, pull-up]
	// 6: Column bit 2 [output, normally low]
	PORTE = _BV(2);
	DDRE = _BV(6);

	// Port F
	// 0-1: Unused     [input, pull-up]
	// 4-7: Row 4-7    [input, pull-up]
	PORTF = _BV(0) | _BV(1) | _BV(4) | _BV(5) | _BV(6) | _BV(7);
	DDRF = 0;

	timer_init();
	
	LED_init();

	TWI_init();
	OLED_init();
	USB_init();

	ENABLE_GLOBAL_INTERRUPTS();
}

u32 loopcount;

void loop() {
	keyboard_update();
	USB_update();
	animation_tick();
}

int main() {
	setup();

	while(1) {
		loop();
	}

	__builtin_unreachable();
}

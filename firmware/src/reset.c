#include "reset.h"

void reset_to_bootloader() {
	// Necessary conditions for jumping to the bootloader on an ATmega32U4
	// are documented by PJRC for the Teensy 2.0
	// https://www.pjrc.com/teensy/jump_to_bootloader.html
	DISABLE_GLOBAL_INTERRUPTS();
	UDCON = 1;
	USBCON = _BV(FRZCLK);
	UCSR1B = 0;
	_delay_ms(5);
    EIMSK = 0;
    PCICR = 0;
    SPCR = 0;
    ACSR = 0;
    EECR = 0;
    ADCSRA = 0;
    TIMSK0 = 0;
    TIMSK1 = 0;
    TIMSK3 = 0;
    TIMSK4 = 0;
    UCSR1B = 0;
    TWCR = 0;
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;
    DDRE = 0;
    DDRF = 0;
    TWCR = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTF = 0;
	asm volatile("jmp 0x7E00");
}

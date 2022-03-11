#include "led.h"

local u8 _led_buffer[3*LED_COUNT] = { 0 };

local const PROGMEM u8 _matrixIndices[ROW_COUNT][COLUMN_COUNT] = {
	{18,  16,  15,  14,  12,  11,  10,  8,   7,   6,   5,   4,   3,   2,   1,   0 },
	{19,  17,  22,  23,  13,  26,  27,  9,   30,  31,  32,  33,  34,  35,  36,  37 },
	{58,  20,  21,  54,  24,  25,  50,  28,  29,  47,  46,  45,  43,  42,  40,  39 },
	{59,  57,  56,  55,  53,  52,  51,  49,  48,  70,  71,  44,  74,  41,  78,  38 },
	{95,  60,  61,  62,  64,  65,  66,  67,  69,  84,  72,  73,  75,  76,  77,  79 },
	{96,  94,  93,  63,  90,  89,  88,  68,  85,  106, 83,  109, 110, 82,  81,  80 },
	{127, 97,  92,  91,  100, 101, 87,  86,  105, 122, 107, 108, 117, 111, 112, 113 },
	{126, 125, 98,  99,  124, 102, 103, 104, 123, 121, 120, 119, 118, 116, 115, 114 }
};

local const PROGMEM u8 _ledCoordinates[LED_COUNT] = { 0 };

void LED_init() {
	LED_update();
}

void LED_clear() {
	memset(_led_buffer, 0, sizeof(_led_buffer));
}

local void _LED_set_colors(u8 *data, size_t length) {
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

void LED_set(u8 n, u8 r, u8 g, u8 b) {
	_led_buffer[3*n] = g;
	_led_buffer[3*n+1] = r;
	_led_buffer[3*n+2] = b;
}

void LED_update() {
	_LED_set_colors(_led_buffer, sizeof(_led_buffer));
}

u8 LED_matrix_to_index(u8 row, u8 column) {
	return pgm_read_byte(&_matrixIndices[row][column]);
}

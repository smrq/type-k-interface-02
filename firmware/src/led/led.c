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

local const PROGMEM u16 _ledCoordinates[LED_COUNT] = {
	0xfc02, 0xe200, 0xcb00, 0xbc00, 0xad00, 0xa000, 0x9100, 0x8500,
	0x7900, 0x6d00, 0x6100, 0x5500, 0x4a00, 0x3e00, 0x3200, 0x2600,
	0x1a00, 0x0e00, 0x0000, 0x000e, 0x0e0e, 0x1a0e, 0x260e, 0x320e,
	0x3e0e, 0x4a0e, 0x550e, 0x610e, 0x6d0e, 0x790e, 0x850e, 0x910e,
	0xa30e, 0xb40e, 0xc00e, 0xcc0e, 0xe20e, 0xfc0e, 0xff1d, 0xf31d,
	0xe71d, 0xdb1d, 0xcc1d, 0xc01d, 0xb41d, 0xa01d, 0x8e1d, 0x821d,
	0x761d, 0x6a1d, 0x5e1d, 0x531d, 0x471d, 0x3b1d, 0x2f1d, 0x231d,
	0x171d, 0x0b1d, 0x001d, 0x0229, 0x1129, 0x1d29, 0x2929, 0x3529,
	0x4129, 0x4d29, 0x5829, 0x6429, 0x7029, 0x7c29, 0x8829, 0x9429,
	0xa329, 0xb429, 0xc029, 0xcc29, 0xdb29, 0xe729, 0xf329, 0xff2f,
	0xf335, 0xe735, 0xdb35, 0x9e35, 0x8b35, 0x7f35, 0x7335, 0x6735,
	0x5b35, 0x5035, 0x4435, 0x3835, 0x2c35, 0x2035, 0x1435, 0x0435,
	0x0741, 0x1a41, 0x2641, 0x3241, 0x3e41, 0x4a41, 0x5541, 0x6141,
	0x6d41, 0x7941, 0x8541, 0x9b41, 0xb441, 0xc041, 0xcc41, 0xdb41,
	0xe741, 0xf341, 0xff47, 0xf34d, 0xe14d, 0xcc4d, 0xc04d, 0xb44d,
	0xa44d, 0x954d, 0x864d, 0x784d, 0x4b4d, 0x1f4d, 0x104d, 0x014d,
};

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

void LED_set(u8 n, u32 rgb) {
	_led_buffer[3*n+0] = (rgb>>8) & 0xFF;
	_led_buffer[3*n+1] = (rgb>>16) & 0xFF;
	_led_buffer[3*n+2] = (rgb>>0) & 0xFF;
}

void LED_set_parametric(u32 (*fn)(u8, u8)) {
	for (u8 n = 0; n < LED_COUNT; ++n) {
		u16 coords = pgm_read_word(&_ledCoordinates[n]);
		u32 rgb = fn(coords >> 8 & 0xFF, coords & 0xFF);
		LED_set(n, rgb);
	}
}

void LED_update() {
	_LED_set_colors(_led_buffer, sizeof(_led_buffer));
}

u8 LED_matrix_to_index(u8 row, u8 column) {
	return pgm_read_byte(&_matrixIndices[row][column]);
}

#include "oled.h"

const PROGMEM u8 initCommand[] = {
	0x00,
	0xAE, // Set Display OFF
	0xD5, 0x81, // Set Display Clock Divide Ratio / Oscillator Frequency
	0x81, 0x7F, // Set Contrast Control for BANK0 = 7F
	0x20, 0x00, // Set Memory Addressing Mode = Horizontal
	0xD3, 0x00, // Set Display Offset = 0
	0x40, // Set Display Start Line = 0
	0xA8, 0x1F, // Set Multiplex Ratio
	0xDA, 0x02, // Set COM Pins Hardware Configuration
	0xA0, // Set Segment Re-map = 0
	0xC0, // Set COM Output Scan Direction
	0xA6, // Set Normal/Inverse Display = Normal
	0x2E, // Deactivate Scroll
	0xD9, 0x02, // Set Pre-charge Period = 2
	0x8D, 0x14, // Enable Charge Pump
	0xAF, // Set Display ON
};

u8 backPage = 1;
u8 framebuffer[1+OLED_WIDTH*OLED_HEIGHT/8] = { 0x40, 0 };

local void _OLED_copy_framebuffer(u8 page) {
	u8 command[] = {
		0x00,
		0x21, 0x00, 0x7F, // Set Column Address
		0x22, (page<<2), ((page+1)<<2)-1, // Set Page Address
	};
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, command, sizeof(command)));
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, framebuffer, sizeof(framebuffer)));
}

local void _OLED_set_visible_page(u8 page) {
	u8 command[] = {
		0x00,
		0x40|(page<<5), // Set Display Start Line
	};
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, command, sizeof(command)));
}

void OLED_init() {
	TWI_BLOCKING(TWI_send_progmem(OLED_ADDRESS, initCommand, sizeof(initCommand)));
	_OLED_copy_framebuffer(0);
	_OLED_copy_framebuffer(1);
}

void OLED_flip() {
	_OLED_copy_framebuffer(backPage);
	_OLED_set_visible_page(backPage);
	backPage = !backPage;
}

void OLED_clear() {
	for (size_t i = 1; i < sizeof(framebuffer); ++i) {
		framebuffer[i] = 0x00;
	}
}

void OLED_test_pattern(u8 n) {
	switch (n) {
		case 0:
			for (size_t i = 1; i < sizeof(framebuffer); i+=2) {
				framebuffer[i] = 0x55;
				framebuffer[i+1] = 0xAA;
			}
			break;
		case 1:
			for (size_t i = 1; i < sizeof(framebuffer); i+=8) {
				framebuffer[i] = 0x0F;
				framebuffer[i+1] = 0x0F;
				framebuffer[i+2] = 0x0F;
				framebuffer[i+3] = 0x0F;
				framebuffer[i+4] = 0xF0;
				framebuffer[i+5] = 0xF0;
				framebuffer[i+6] = 0xF0;
				framebuffer[i+7] = 0xF0;
			}
			break;
		case 2:
			for (size_t i = 1; i < sizeof(framebuffer); i+=8) {
				framebuffer[i] = 0x03;
				framebuffer[i+1] = 0x06;
				framebuffer[i+2] = 0x0C;
				framebuffer[i+3] = 0x18;
				framebuffer[i+4] = 0x30;
				framebuffer[i+5] = 0x60;
				framebuffer[i+6] = 0xC0;
				framebuffer[i+7] = 0x81;
			}
			break;
	}
}

void OLED_draw_pixel(i16 x, i16 y, bool color) {
	if (x < 0 || x >= OLED_WIDTH || y < 0 || y >= OLED_HEIGHT) return;
	if (color) {
		framebuffer[1 + x + OLED_WIDTH*(y>>3)] |= (1<<(y % 8));
	} else {
		framebuffer[1 + x + OLED_WIDTH*(y>>3)] &= ~(1<<(y % 8));
	}
}

void OLED_draw_horizontal_line(i16 x0, i16 x1, i16 y, bool color) {
	if (y < 0 || y >= OLED_HEIGHT || x1 < 0 || x0 >= OLED_WIDTH) return;
	for (i16 x = x0; x <= x1; ++x) {
		OLED_draw_pixel(x, y, color);
	}
}

void OLED_draw_vertical_line(i16 x, i16 y0, i16 y1, bool color) {
	if (x < 0 || x >= OLED_WIDTH || y1 < 0 || y0 >= OLED_HEIGHT) return;
	for (i16 y = y0; y <= y1; ++y) {
		OLED_draw_pixel(x, y, color);
	}
}

void OLED_draw_line(i16 x0, i16 y0, i16 x1, i16 y1, bool color) {
	if (y0 == y1) {
		OLED_draw_horizontal_line(x0, x1, y0, color);
	} else if (x0 == x1) {
		OLED_draw_vertical_line(x0, y0, y1, color);
	} else {
		// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
		i16 dx = x0 < x1 ? x1 - x0 : x0 - x1;
		i16 sx = x0 < x1 ? 1 : -1;
		i16 dy = y0 < y1 ? y0 - y1 : y1 - y0;
		i16 sy = y0 < y1 ? 1 : -1;
		i16 error = dx + dy;

		while (true) {
			OLED_draw_pixel(x0, y0, color);
			if (x0 == x1 && y0 == y1) break;
			i16 e2 = 2*error;

			if (e2 >= dy) {
				if (x0 == x1) break;
				error += dy;
				x0 += sx;
			}
			if (e2 <= dx) {
				if (y0 == y1) break;
				error += dx;
				y0 += sy;
			}
		}
	}
}

void OLED_draw_rectangle(i16 x0, i16 y0, i16 x1, i16 y1, bool color) {
	OLED_draw_horizontal_line(x0, x1, y0, color);
	OLED_draw_horizontal_line(x0, x1, y1, color);
	OLED_draw_vertical_line(x0, y0, y1, color);
	OLED_draw_vertical_line(x1, y0, y1, color);
}

void OLED_draw_filled_rectangle(i16 x0, i16 y0, i16 x1, i16 y1, bool color) {
	for (i16 y = y0; y <= y1; ++y) {
		OLED_draw_horizontal_line(x0, x1, y, color);
	}
}

void OLED_draw_circle(i16 x0, i16 y0, i16 r, bool color) {
	// Use donut code to save space
	OLED_draw_filled_donut(x0, y0, r, r, color);
}

void OLED_draw_filled_circle(i16 x0, i16 y0, i16 r, bool color) {
	// Implemented in terms of donut to save space
	OLED_draw_filled_donut(x0, y0, r, 0, color);
}

local void _OLED_draw_donut_octant(i16 x0, i16 y0, i16 x, i16 yi, i16 yo, bool color) {
	OLED_draw_vertical_line(x0-x, y0-yo, y0-yi, color);
	OLED_draw_vertical_line(x0-x, y0+yi, y0+yo, color);
	OLED_draw_vertical_line(x0+x, y0-yo, y0-yi, color);
	OLED_draw_vertical_line(x0+x, y0+yi, y0+yo, color);
	OLED_draw_horizontal_line(x0-yo, x0-yi, y0-x, color);
	OLED_draw_horizontal_line(x0+yi, x0+yo, y0-x, color);
	OLED_draw_horizontal_line(x0-yo, x0-yi, y0+x, color);
	OLED_draw_horizontal_line(x0+yi, x0+yo, y0+x, color);
}

void OLED_draw_filled_donut(i16 x0, i16 y0, i16 rOuter, i16 rInner, bool color) {
	i16 x = 0;
	i16 yi = rInner;
	i16 yo = rOuter;

	i16 fo = 1-rOuter;
	i16 fi = 1-rInner;
	i16 ddf_x = 1;
	i16 ddfo_y = -2*rOuter;
	i16 ddfi_y = -2*rInner;

	_OLED_draw_donut_octant(x0, y0, 0, rInner, rOuter, color);

	while (x < yi) {
		if (fo >= 0) {
			--yo;
			ddfo_y += 2;
			fo += ddfo_y;
		}
		if (fi >= 0) {
			--yi;
			ddfi_y += 2;
			fi += ddfi_y;
		}
		++x;
		ddf_x += 2;
		fo += ddf_x;
		fi += ddf_x;

		_OLED_draw_donut_octant(x0, y0, x, yi, yo, color);
	}

	while (x < yo) {
		if (fo >= 0) {
			--yo;
			ddfo_y += 2;
			fo += ddfo_y;
		}
		++yi;
		++x;
		ddf_x += 2;
		fo += ddf_x;

		_OLED_draw_donut_octant(x0, y0, x, yi, yo, color);
	}
}

i16 OLED_draw_char(i16 x, i16 y, char c, const u16 *font, const u16 *fontOffset, bool color) {
	u16 i = pgm_read_word(&fontOffset[c - 0x20]);
	u16 end = pgm_read_word(&fontOffset[c - 0x20 + 1]);
	for (; i < end; ++i, ++x) {
		if (x >= OLED_WIDTH) break;
		if (x < 0) continue;

		u32 pattern = pgm_read_word(font + i);
		u8 yByte;

		if (y < 0) {
			pattern = pattern >> -y;
			yByte = 0;
		} else {
			pattern = pattern << (y % 8);
			yByte = y >> 3;
		}

		while (pattern && yByte < (OLED_HEIGHT >> 3)) {
			if (color) {
				framebuffer[1 + x + OLED_WIDTH*yByte] |= pattern & 0xFF;
			} else {
				framebuffer[1 + x + OLED_WIDTH*yByte] &= ~(pattern & 0xFF);
			}
			pattern = pattern >> 8;
			++yByte;
		}
	}
	return x;
}

i16 OLED_draw_text(i16 x, i16 y, const char *text, const u16 *font, const u16 *fontOffset, bool color) {
	while (*text) {
		x = OLED_draw_char(x, y, *text++, font, fontOffset, color);
	}
	return x;
}

void OLED_scroll_vertical(i16 dy) {
	for (i16 x = 0; x < OLED_WIDTH; ++x) {
		u32 column =
			((u32)framebuffer[1 + x]) |
			((u32)framebuffer[1 + x + OLED_WIDTH] << 8) |
			((u32)framebuffer[1 + x + 2*OLED_WIDTH] << 16) |
			((u32)framebuffer[1 + x + 3*OLED_WIDTH] << 24);
		if (dy > 0) {
			column = column >> dy;
		} else {
			column = column >> -dy;
		}
		framebuffer[1 + x] = column & 0xFF;
		framebuffer[1 + x + OLED_WIDTH] = (column >> 8) & 0xFF;
		framebuffer[1 + x + 2*OLED_WIDTH] = (column >> 16) & 0xFF;
		framebuffer[1 + x + 3*OLED_WIDTH] = (column >> 24) & 0xFF;
	}
}

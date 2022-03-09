#pragma once

#include "../defs.h"
#include "../font/fonts.h"
#include "twi.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define OLED_ADDRESS 0x3C

void OLED_init();
void OLED_flip();
void OLED_clear();
void OLED_test_pattern(u8 n);
void OLED_draw_pixel(i16 x, i16 y, bool color);
void OLED_draw_horizontal_line(i16 x0, i16 x1, i16 y, bool color);
void OLED_draw_vertical_line(i16 x, i16 y0, i16 y1, bool color);
void OLED_draw_line(i16 x0, i16 y0, i16 x1, i16 y1, bool color);
void OLED_draw_rectangle(i16 x0, i16 y0, i16 x1, i16 y1, bool color);
void OLED_draw_filled_rectangle(i16 x0, i16 y0, i16 x1, i16 y1, bool color);
void OLED_draw_circle(i16 x, i16 y, i16 r, bool color);
void OLED_draw_filled_circle(i16 x, i16 y, i16 r, bool color);
void OLED_draw_filled_donut(i16 x0, i16 y0, i16 rOuter, i16 rInner, bool color);
i16 OLED_draw_char(i16 x, i16 y, char c, const u16 *font, const u16 *fontOffset, bool color);
i16 OLED_draw_text(i16 x, i16 y, const char *text, const u16 *font, const u16 *fontOffset, bool color);
void OLED_scroll_vertical(i16 dy);

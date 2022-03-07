#pragma once

#include "defs.h"
#include "twi.h"

#include "led.h" // for testing

void OLED_init();
void OLED_flip();
void OLED_clear();
void OLED_test_pattern(u8 n);
void OLED_draw_pixel(i16 x, i16 y, u8 color);
void OLED_draw_horizontal_line(i16 x0, i16 x1, i16 y, u8 color);
void OLED_draw_vertical_line(i16 x, i16 y0, i16 y1, u8 color);
void OLED_draw_line(i16 x0, i16 y0, i16 x1, i16 y1, u8 color);
void OLED_draw_rectangle(i16 x0, i16 y0, i16 x1, i16 y1, u8 color);
void OLED_draw_filled_rectangle(i16 x0, i16 y0, i16 x1, i16 y1, u8 color);
void OLED_draw_circle(i16 x, i16 y, i16 r, u8 color);
void OLED_draw_filled_circle(i16 x, i16 y, i16 r, u8 color);
#include "animation.h"

void animation_drop() {
	OLED_clear();
	OLED_draw_pixel(64, 16, true);
	OLED_flip();
	_delay_ms(120);

	OLED_draw_filled_circle(64, 16, 1, true);
	OLED_flip();
	_delay_ms(80);

	OLED_draw_filled_donut(64, 16, 18, 10, true);
	OLED_flip();
	_delay_ms(80);

	OLED_clear();
	OLED_draw_filled_donut(64, 16, 35, 30, true);
	OLED_draw_filled_circle(64, 16, 1, true);
	OLED_flip();
	_delay_ms(60);

	OLED_clear();
	OLED_draw_filled_donut(64, 16, 40, 37, true);
	OLED_draw_filled_circle(64, 16, 1, true);
	OLED_flip();
	_delay_ms(40);

	OLED_clear();
	OLED_draw_filled_donut(64, 16, 45, 43, true);
	OLED_draw_filled_circle(64, 16, 1, true);
	OLED_flip();
	_delay_ms(40);

	OLED_clear();
	OLED_draw_filled_donut(64, 16, 47, 45, true);
	OLED_draw_filled_circle(64, 16, 1, true);
	OLED_flip();
	_delay_ms(40);

	OLED_clear();
	OLED_draw_circle(64, 16, 49, true);
	OLED_draw_filled_circle(64, 16, 1, true);
	OLED_flip();
	_delay_ms(40);

	OLED_clear();
	OLED_draw_circle(64, 16, 50, true);
	OLED_draw_filled_circle(64, 16, 1, true);
	OLED_flip();
	_delay_ms(100);

	OLED_clear();
	OLED_draw_pixel(64, 16, true);
	OLED_flip();
	_delay_ms(100);

	OLED_clear();
	OLED_flip();
}

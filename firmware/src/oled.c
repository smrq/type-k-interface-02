#include "oled.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define OLED_ADDRESS 0x3C

PROGMEM const u8 initCommand[] = {
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
	0xC0, // Set COM Output Scan Direction = 0
	0xA6, // Set Normal/Inverse Display = Normal
	0x2E, // Deactivate Scroll
	0xD9, 0x02, // Set Pre-charge Period = 2
	0x8D, 0x14, // Enable Charge Pump
	0xAF, // Set Display ON
	0x21, 0x00, 0x7F, // Set Column Address
};

u8 writeSide = 1;
u8 framebuffer[1+128*32/8] = { 0x40, 0 };

void OLED_init() {
	TWI_BLOCKING(TWI_send_progmem(OLED_ADDRESS, initCommand, sizeof(initCommand)));
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, (u8[]){ 0x00, 0x22, 0x00, 0x03 }, 4)); // Set Page Address
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, framebuffer, sizeof(framebuffer)));
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, (u8[]){ 0x00, 0x22, 0x04, 0x07 }, 4)); // Set Page Address
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, framebuffer, sizeof(framebuffer)));
}

void OLED_flip() {
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, (u8[]){ 0x00, 0x22, writeSide<<2, ((writeSide+1)<<2)-1}, 4)); // Set Page Address
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, framebuffer, sizeof(framebuffer)));
	TWI_BLOCKING(TWI_send(OLED_ADDRESS, (u8[]){ 0x00, 0x40 | (writeSide<<5) }, 3));
	writeSide = !writeSide;
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

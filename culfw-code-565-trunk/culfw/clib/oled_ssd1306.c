
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include "oled_ssd1306.h"
#include "i2cmaster.h"
#include "delay.h"

// Standard ASCII 5x7 font
const uint8_t oled_font[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
	0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
	0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
	0x18, 0x3C, 0x7E, 0x3C, 0x18,
	0x1C, 0x57, 0x7D, 0x57, 0x1C,
	0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
	0x00, 0x18, 0x3C, 0x18, 0x00,
	0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
	0x00, 0x18, 0x24, 0x18, 0x00,
	0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
	0x30, 0x48, 0x3A, 0x06, 0x0E,
	0x26, 0x29, 0x79, 0x29, 0x26,
	0x40, 0x7F, 0x05, 0x05, 0x07,
	0x40, 0x7F, 0x05, 0x25, 0x3F,
	0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
	0x7F, 0x3E, 0x1C, 0x1C, 0x08,
	0x08, 0x1C, 0x1C, 0x3E, 0x7F,
	0x14, 0x22, 0x7F, 0x22, 0x14,
	0x5F, 0x5F, 0x00, 0x5F, 0x5F,
	0x06, 0x09, 0x7F, 0x01, 0x7F,
	0x00, 0x66, 0x89, 0x95, 0x6A,
	0x60, 0x60, 0x60, 0x60, 0x60,
	0x94, 0xA2, 0xFF, 0xA2, 0x94,
	0x08, 0x04, 0x7E, 0x04, 0x08,
	0x10, 0x20, 0x7E, 0x20, 0x10,
	0x08, 0x08, 0x2A, 0x1C, 0x08,
	0x08, 0x1C, 0x2A, 0x08, 0x08,
	0x1E, 0x10, 0x10, 0x10, 0x10,
	0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
	0x30, 0x38, 0x3E, 0x38, 0x30,
	0x06, 0x0E, 0x3E, 0x0E, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x5F, 0x00, 0x00,
	0x00, 0x07, 0x00, 0x07, 0x00,
	0x14, 0x7F, 0x14, 0x7F, 0x14,
	0x24, 0x2A, 0x7F, 0x2A, 0x12,
	0x23, 0x13, 0x08, 0x64, 0x62,
	0x36, 0x49, 0x56, 0x20, 0x50,
	0x00, 0x08, 0x07, 0x03, 0x00,
	0x00, 0x1C, 0x22, 0x41, 0x00,
	0x00, 0x41, 0x22, 0x1C, 0x00,
	0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
	0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x80, 0x70, 0x30, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x60, 0x60, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E,
	0x00, 0x42, 0x7F, 0x40, 0x00,
	0x72, 0x49, 0x49, 0x49, 0x46,
	0x21, 0x41, 0x49, 0x4D, 0x33,
	0x18, 0x14, 0x12, 0x7F, 0x10,
	0x27, 0x45, 0x45, 0x45, 0x39,
	0x3C, 0x4A, 0x49, 0x49, 0x31,
	0x41, 0x21, 0x11, 0x09, 0x07,
	0x36, 0x49, 0x49, 0x49, 0x36,
	0x46, 0x49, 0x49, 0x29, 0x1E,
	0x00, 0x00, 0x14, 0x00, 0x00,
	0x00, 0x40, 0x34, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x00, 0x41, 0x22, 0x14, 0x08,
	0x02, 0x01, 0x59, 0x09, 0x06,
	0x3E, 0x41, 0x5D, 0x59, 0x4E,
	0x7C, 0x12, 0x11, 0x12, 0x7C,
	0x7F, 0x49, 0x49, 0x49, 0x36,
	0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x49, 0x49, 0x49, 0x41,
	0x7F, 0x09, 0x09, 0x09, 0x01,
	0x3E, 0x41, 0x41, 0x51, 0x73,
	0x7F, 0x08, 0x08, 0x08, 0x7F,
	0x00, 0x41, 0x7F, 0x41, 0x00,
	0x20, 0x40, 0x41, 0x3F, 0x01,
	0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40,
	0x7F, 0x02, 0x1C, 0x02, 0x7F,
	0x7F, 0x04, 0x08, 0x10, 0x7F,
	0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06,
	0x3E, 0x41, 0x51, 0x21, 0x5E,
	0x7F, 0x09, 0x19, 0x29, 0x46,
	0x26, 0x49, 0x49, 0x49, 0x32,
	0x03, 0x01, 0x7F, 0x01, 0x03,
	0x3F, 0x40, 0x40, 0x40, 0x3F,
	0x1F, 0x20, 0x40, 0x20, 0x1F,
	0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x03, 0x04, 0x78, 0x04, 0x03,
	0x61, 0x59, 0x49, 0x4D, 0x43,
	0x00, 0x7F, 0x41, 0x41, 0x41,
	0x02, 0x04, 0x08, 0x10, 0x20,
	0x00, 0x41, 0x41, 0x41, 0x7F,
	0x04, 0x02, 0x01, 0x02, 0x04,
	0x40, 0x40, 0x40, 0x40, 0x40,
	0x00, 0x03, 0x07, 0x08, 0x00,
	0x20, 0x54, 0x54, 0x78, 0x40,
	0x7F, 0x28, 0x44, 0x44, 0x38,
	0x38, 0x44, 0x44, 0x44, 0x28,
	0x38, 0x44, 0x44, 0x28, 0x7F,
	0x38, 0x54, 0x54, 0x54, 0x18,
	0x00, 0x08, 0x7E, 0x09, 0x02,
	0x18, 0xA4, 0xA4, 0x9C, 0x78,
	0x7F, 0x08, 0x04, 0x04, 0x78,
	0x00, 0x44, 0x7D, 0x40, 0x00,
	0x20, 0x40, 0x40, 0x3D, 0x00,
	0x7F, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x41, 0x7F, 0x40, 0x00,
	0x7C, 0x04, 0x78, 0x04, 0x78,
	0x7C, 0x08, 0x04, 0x04, 0x78,
	0x38, 0x44, 0x44, 0x44, 0x38,
	0xFC, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0xFC,
	0x7C, 0x08, 0x04, 0x04, 0x08,
	0x48, 0x54, 0x54, 0x54, 0x24,
	0x04, 0x04, 0x3F, 0x44, 0x24,
	0x3C, 0x40, 0x40, 0x20, 0x7C,
	0x1C, 0x20, 0x40, 0x20, 0x1C,
	0x3C, 0x40, 0x30, 0x40, 0x3C,
	0x44, 0x28, 0x10, 0x28, 0x44,
	0x4C, 0x90, 0x90, 0x90, 0x7C,
	0x44, 0x64, 0x54, 0x4C, 0x44,
	0x00, 0x08, 0x36, 0x41, 0x00,
	0x00, 0x00, 0x77, 0x00, 0x00,
	0x00, 0x41, 0x36, 0x08, 0x00,
	0x02, 0x01, 0x02, 0x04, 0x02,
	0x3C, 0x26, 0x23, 0x26, 0x3C,
	0x1E, 0xA1, 0xA1, 0x61, 0x12,
	0x3A, 0x40, 0x40, 0x20, 0x7A,
	0x38, 0x54, 0x54, 0x55, 0x59,
	0x21, 0x55, 0x55, 0x79, 0x41,
	0x22, 0x54, 0x54, 0x78, 0x42, // a-umlaut
	0x21, 0x55, 0x54, 0x78, 0x40,
	0x20, 0x54, 0x55, 0x79, 0x40,
	0x0C, 0x1E, 0x52, 0x72, 0x12,
	0x39, 0x55, 0x55, 0x55, 0x59,
	0x39, 0x54, 0x54, 0x54, 0x59,
	0x39, 0x55, 0x54, 0x54, 0x58,
	0x00, 0x00, 0x45, 0x7C, 0x41,
	0x00, 0x02, 0x45, 0x7D, 0x42,
	0x00, 0x01, 0x45, 0x7C, 0x40,
	0x7D, 0x12, 0x11, 0x12, 0x7D, // A-umlaut
	0xF0, 0x28, 0x25, 0x28, 0xF0,
	0x7C, 0x54, 0x55, 0x45, 0x00,
	0x20, 0x54, 0x54, 0x7C, 0x54,
	0x7C, 0x0A, 0x09, 0x7F, 0x49,
	0x32, 0x49, 0x49, 0x49, 0x32,
	0x3A, 0x44, 0x44, 0x44, 0x3A, // o-umlaut
	0x32, 0x4A, 0x48, 0x48, 0x30,
	0x3A, 0x41, 0x41, 0x21, 0x7A,
	0x3A, 0x42, 0x40, 0x20, 0x78,
	0x00, 0x9D, 0xA0, 0xA0, 0x7D,
	0x3D, 0x42, 0x42, 0x42, 0x3D, // O-umlaut
	0x3D, 0x40, 0x40, 0x40, 0x3D,
	0x3C, 0x24, 0xFF, 0x24, 0x24,
	0x48, 0x7E, 0x49, 0x43, 0x66,
	0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
	0xFF, 0x09, 0x29, 0xF6, 0x20,
	0xC0, 0x88, 0x7E, 0x09, 0x03,
	0x20, 0x54, 0x54, 0x79, 0x41,
	0x00, 0x00, 0x44, 0x7D, 0x41,
	0x30, 0x48, 0x48, 0x4A, 0x32,
	0x38, 0x40, 0x40, 0x22, 0x7A,
	0x00, 0x7A, 0x0A, 0x0A, 0x72,
	0x7D, 0x0D, 0x19, 0x31, 0x7D,
	0x26, 0x29, 0x29, 0x2F, 0x28,
	0x26, 0x29, 0x29, 0x29, 0x26,
	0x30, 0x48, 0x4D, 0x40, 0x20,
	0x38, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x38,
	0x2F, 0x10, 0xC8, 0xAC, 0xBA,
	0x2F, 0x10, 0x28, 0x34, 0xFA,
	0x00, 0x00, 0x7B, 0x00, 0x00,
	0x08, 0x14, 0x2A, 0x14, 0x22,
	0x22, 0x14, 0x2A, 0x14, 0x08,
	0x55, 0x00, 0x55, 0x00, 0x55, // #176 (25% block) missing in old code
	0xAA, 0x55, 0xAA, 0x55, 0xAA, // 50% block
	0xFF, 0x55, 0xFF, 0x55, 0xFF, // 75% block
	0x00, 0x00, 0x00, 0xFF, 0x00,
	0x10, 0x10, 0x10, 0xFF, 0x00,
	0x14, 0x14, 0x14, 0xFF, 0x00,
	0x10, 0x10, 0xFF, 0x00, 0xFF,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x14, 0x14, 0x14, 0xFC, 0x00,
	0x14, 0x14, 0xF7, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x14, 0x14, 0xF4, 0x04, 0xFC,
	0x14, 0x14, 0x17, 0x10, 0x1F,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0x1F, 0x00,
	0x10, 0x10, 0x10, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0xF0, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x10, 0xFF, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x14,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0x00, 0x1F, 0x10, 0x17,
	0x00, 0x00, 0xFC, 0x04, 0xF4,
	0x14, 0x14, 0x17, 0x10, 0x17,
	0x14, 0x14, 0xF4, 0x04, 0xF4,
	0x00, 0x00, 0xFF, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x14, 0x14, 0xF7, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x17, 0x14,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0xF4, 0x14,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x00, 0x00, 0x1F, 0x10, 0x1F,
	0x00, 0x00, 0x00, 0x1F, 0x14,
	0x00, 0x00, 0x00, 0xFC, 0x14,
	0x00, 0x00, 0xF0, 0x10, 0xF0,
	0x10, 0x10, 0xFF, 0x10, 0xFF,
	0x14, 0x14, 0x14, 0xFF, 0x14,
	0x10, 0x10, 0x10, 0x1F, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0x10,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0xFF, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x38, 0x44, 0x44, 0x38, 0x44,
	0xFC, 0x4A, 0x4A, 0x4A, 0x34, // sharp-s or beta
	0x7E, 0x02, 0x02, 0x06, 0x06,
	0x02, 0x7E, 0x02, 0x7E, 0x02,
	0x63, 0x55, 0x49, 0x41, 0x63,
	0x38, 0x44, 0x44, 0x3C, 0x04,
	0x40, 0x7E, 0x20, 0x1E, 0x20,
	0x06, 0x02, 0x7E, 0x02, 0x02,
	0x99, 0xA5, 0xE7, 0xA5, 0x99,
	0x1C, 0x2A, 0x49, 0x2A, 0x1C,
	0x4C, 0x72, 0x01, 0x72, 0x4C,
	0x30, 0x4A, 0x4D, 0x4D, 0x30,
	0x30, 0x48, 0x78, 0x48, 0x30,
	0xBC, 0x62, 0x5A, 0x46, 0x3D,
	0x3E, 0x49, 0x49, 0x49, 0x00,
	0x7E, 0x01, 0x01, 0x01, 0x7E,
	0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
	0x44, 0x44, 0x5F, 0x44, 0x44,
	0x40, 0x51, 0x4A, 0x44, 0x40,
	0x40, 0x44, 0x4A, 0x51, 0x40,
	0x00, 0x00, 0xFF, 0x01, 0x03,
	0xE0, 0x80, 0xFF, 0x00, 0x00,
	0x08, 0x08, 0x6B, 0x6B, 0x08,
	0x36, 0x12, 0x36, 0x24, 0x36,
	0x06, 0x0F, 0x09, 0x0F, 0x06,
	0x00, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x00, 0x10, 0x10, 0x00,
	0x30, 0x40, 0xFF, 0x01, 0x01,
	0x00, 0x1F, 0x01, 0x01, 0x1E,
	0x00, 0x19, 0x1D, 0x17, 0x12,
	0x00, 0x3C, 0x3C, 0x3C, 0x3C,
	0x00, 0x00, 0x00, 0x00, 0x00  // #255 NBSP
};


uint8_t oled_buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};




// 16x16 bitmap logo Apple Homekit
// That is 2 bytes high, and 16 bytes wide.
const uint8_t logoAppleHomekit[32] PROGMEM = {
	0b00000000, 0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000010, 0b00000100, 0b00001000, 0b00011100, 0b00000100, 0b01111100, 0b10000000, 0b00000000,
	0b00000001, 0b00000001, 0b11111111, 0b10000000, 0b10000010, 0b10001001, 0b10000101, 0b10010101, 0b10010101, 0b10000101, 0b10001001, 0b10000010, 0b10000000, 0b11111111, 0b00000001, 0b00000001
};

const uint8_t logoFHEM[32] PROGMEM = {
	0b00110000, 0b11110000, 0b11111000, 0b00011000, 0b00001100, 0b01001100, 0b01001110, 0b00000110, 0b00000110, 0b11000110, 0b10001100, 0b00011000, 0b00110000, 0b11110000, 0b11100000, 0b11000000,
	0b00110000, 0b01111111, 0b00000000, 0b01000000, 0b01000000, 0b01001110, 0b11001000, 0b11011000, 0b11010000, 0b11010000, 0b11011000, 0b10001100, 0b10000000, 0b11111000, 0b01111111, 0b00000000
};

const uint8_t logoHeater[32] PROGMEM = {
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b11100000, 0b11100000, 0b11110000, 0b11111110, 0b11111110, 0b11111100, 0b11111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b11000000, 0b11000000, 0b11000011, 0b11000111, 0b11001111, 0b11011111, 0b11011111, 0b11000011, 0b11000001, 0b11000000, 0b11000000, 0b11000000, 0b00000000, 0b00000000
};
/*
const uint8_t logoHeaterOff[32] PROGMEM = {
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
	0b00000000, 0b00000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11011000, 0b11011100, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b00000000, 0b00000000
};
*/

int16_t oled_cursor_x, oled_cursor_y;
uint16_t oled_text_color, oled_text_bgcolor;
uint8_t oled_text_size;
uint8_t oled_addr_write;
uint8_t oled_enabled;


uint8_t i2c_reset(void) {
	TWCR &= ~(_BV(TWEN)); //Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
	TWCR |= _BV(TWINT); // "the TWINT bit should be cleared (by writing it to one)"
/*
	// SCL & SDA as input with pull-ups enabled
	DDRD &= ~0x01; // clear bit 0. set as input
	PORTD |= 0x01; // SCL on AVR pin D0
	my_delay_us(50);
	DDRD &= ~0x02; // clear bit 1. set as input
	PORTD |= 0x02; // SDA on AVR pin D1
	// Check is SCL is Low. If it is held low, the AVR cannot become the I2C master
	uint8_t SCL_LOW = PIND & PD0;
	if (SCL_LOW == 0) return 1; // I2C bus error. Could not clear SCL clock line held low
	// Check SDA input
	uint8_t SDA_LOW = PIND & PD1;
	uint8_t clockCount = 20; // > 2x9 clock
	// If SDA is Low
	while(SDA_LOW == 0 && (clockCount > 0)) {
		clockCount--;
		// Note: I2C bus is open collector so do NOT drive SCL or SDA high.
		DDRD &= ~0x01; // release SCL pullup so that when made output it will be LOW
		DDRD |= 0x01; // then clock SCL Low
		my_delay_us(20); // for >5uS
		DDRD &= ~0x01; // release SCL LOW
		PORTD |= 0x01; // turn on pullup resistors again
		// do not force high as slave may be holding it low for clock stretching
		my_delay_us(20); // The >5uS is so that even the slowest I2C devices are handled
		SCL_LOW = PIND & PD1; // Check if SCL is Low
		uint8_t counter = 20;
		while(SCL_LOW == 0 && (counter > 0)) { // loop waiting for SCL to become High only wait 2sec
			counter--;
			my_delay_ms(100);
			SCL_LOW = PIND & PD1; // Check if SCL is Low
		}
		if (SCL_LOW == 0) return 2; // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
		SDA_LOW = PIND & PD1;
	}
	if (SDA_LOW == 0) return 3; // I2C bus error. Could not clear. SDA data line held low
	// else pull SDA line low for Start or Repeated Start
	DDRD &= ~0x02; // set as input. remove pullup
	DDRD |= 0x02; // and then make it LOW i.e. send an I2C Start or Repeated start control
	// When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus
	my_delay_us(20);
	DDRD &= ~0x02; // remove output low
	PORTD |= 0x02; // and make SDA high i.e. send I2C STOP control
	my_delay_us(20);
	DDRD &= ~0x02; // and reset pins as tri-state inputs which is the default state on reset
	DDRD &= ~0x01; // scl too
*/
	// the bus has been cleared..
	i2c_init();
	return 0;
}


uint8_t oled_command(uint8_t c) {
	uint8_t data = 0x00;   // Co = 0, D/C = 0
	if (i2c_start(oled_addr_write) != 0) goto failure;
	if (i2c_write(data) != 0) goto failure;
	if (i2c_write(c) != 0) goto failure;
	i2c_stop();
	return 0;
failure:
	i2c_reset();
	return 1;
}


// c is SSD1306_DISPLAYOFF or SSD1306_DISPLAYON
uint8_t oled_enable(uint8_t c) {
	if (c == SSD1306_DISPLAYON && oled_enabled == 1) return 0; // already on
	if (c == SSD1306_DISPLAYOFF && oled_enabled == 0) return 0; // already off
	oled_enabled = (c == SSD1306_DISPLAYON)? 1 : 0;
	return oled_command(c);
}


void oled_init(void) {
	// activate internal pullups for the two wire interface.
//	DDRD &= ~0x03; // clear bits 0 & 1. set as input
//	PORTD |= 0x03; // SCL on AVR pin D0, SDA on AVR pin D1
	oled_addr_write = (SSD1306_I2C_ADDRESS << 1) | I2C_WRITE;
	oled_enabled = 1;
	i2c_init();
	// Init sequence
	oled_command(SSD1306_DISPLAYOFF);					// 0xAE
	oled_command(SSD1306_SETDISPLAYCLOCKDIV);			// 0xD5
	oled_command(0x80);									// the suggested ratio 0x80
	oled_command(SSD1306_SETMULTIPLEX);					// 0xA8
	oled_command(SSD1306_LCDHEIGHT - 1);
	oled_command(SSD1306_SETDISPLAYOFFSET);				// 0xD3
	oled_command(0x0);									// no offset
	oled_command(SSD1306_SETSTARTLINE | 0x0);			// line #0
	oled_command(SSD1306_CHARGEPUMP);					// 0x8D
	oled_command(0x14);									// SSD1306_SWITCHCAPVCC
	oled_command(SSD1306_MEMORYMODE);					// 0x20
	oled_command(0x00);									// 0x0 act like ks0108
	oled_command(SSD1306_SEGREMAP | 0x1);
	oled_command(SSD1306_COMSCANDEC);
	// 128x32
	oled_command(SSD1306_SETCOMPINS);					// 0xDA
	oled_command(0x02);
	oled_command(SSD1306_SETCONTRAST);					// 0x81
	oled_command(0x8F);
	oled_command(SSD1306_SETPRECHARGE);					// 0xd9
	oled_command(0xF1);									// SSD1306_SWITCHCAPVCC
	oled_command(SSD1306_SETVCOMDETECT);					// 0xDB
	oled_command(0x40);
	oled_command(SSD1306_DISPLAYALLON_RESUME);			// 0xA4
	oled_command(SSD1306_NORMALDISPLAY);				// 0xA6
	oled_command(SSD1306_DEACTIVATE_SCROLL);
	oled_command(SSD1306_DISPLAYON);					//--turn on oled panel
	// display splashscreen
	oled_display();
	// cursor, colors and size
	oled_setCursor(0,0);
	oled_setTextColors(WHITE, BLACK);
	oled_setTextSize(2);
}


uint8_t oled_display(void) {
	if (oled_command(SSD1306_COLUMNADDR) != 0) goto failure;
	if (oled_command(0) != 0) goto failure;						// Column start address (0 = reset)
	if (oled_command(SSD1306_LCDWIDTH-1) != 0) goto failure;	// Column end address (127 = reset)
	if (oled_command(SSD1306_PAGEADDR) != 0) goto failure;
	if (oled_command(0) != 0) goto failure;						// Page start address (0 = reset)
	if (oled_command(3) != 0) goto failure;						// Page end address, LCDHEIGHT == 32
	// write in chunks..
	uint8_t setstartline = SSD1306_SETSTARTLINE; // 0x40
	for (uint16_t i=0; i<SSD1306_LCDBYTES;) {
		if (i2c_start(oled_addr_write) != 0) goto failurex; // i2c_start_wait	(data)
		if (i2c_write(setstartline) != 0) goto failurex;
		for (uint8_t x=0; x<32; x++) {
			if (i2c_write(oled_buffer[i]) != 0) goto failurex;
			i++;
		}
		i2c_stop();
	}
	return 0;
failurex:
	i2c_reset();
failure:
	return 1;
}


void oled_cls(void) {
	memset(oled_buffer, 0, SSD1306_LCDBYTES);
}


void oled_setCursor(int16_t x, int16_t y) {
	oled_cursor_x = x;
	oled_cursor_y = y;
}


void oled_setTextColor(uint16_t c) {
	// For 'transparent' background, we'll set the bg
	// to the same as fg instead of using a flag
	oled_text_color = oled_text_bgcolor = c;
}


void oled_setTextColors(uint16_t c, uint16_t bg) {
	oled_text_color = c;
	oled_text_bgcolor = bg;
}

	
void oled_setTextSize(uint8_t s) {
	oled_text_size = (s > 0) ? s : 1;
}


void oled_drawFastVLine(int16_t x, int16_t __y, int16_t __h, uint16_t color) {
	// do nothing if we're off the left or right side of the screen
	if (x < 0 || x >= SSD1306_LCDWIDTH) return;
	// make sure we don't try to draw below 0
	if (__y < 0) {
		// __y is negative, this will subtract enough from __h to account for __y being 0
		__h += __y;
		__y = 0;
	}
	// make sure we don't go past the height of the display
	if ((__y + __h) > SSD1306_LCDHEIGHT) __h = (SSD1306_LCDHEIGHT - __y);
	// if our height is now negative, punt
	if (__h <= 0) return;
	// this display doesn't need ints for coordinates, use local byte registers for faster juggling
	register uint8_t y = __y;
	register uint8_t h = __h;
	// set up the pointer for fast movement through the buffer
	register uint8_t *pBuf = oled_buffer;
	// adjust the buffer pointer for the current row
	pBuf += ((y/8) * SSD1306_LCDWIDTH);
	// and offset x columns in
	pBuf += x;
	// do the first partial byte, if necessary - this requires some masking
	register uint8_t mod = (y&7);
	if (mod) {
		// mask off the high n bits we want to set
		mod = 8 - mod;
		// note - lookup table results in a nearly 10% performance improvement in fill* functions
		// register uint8_t mask = ~(0xFF >> (mod));
		static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
		register uint8_t mask = premask[mod];
		// adjust the mask if we're not going to reach the end of this byte
		if (h < mod) mask &= (0XFF >> (mod-h));
		switch(color) {
			case WHITE:		*pBuf |=  mask; break;
			case BLACK:		*pBuf &= ~mask; break;
			case INVERSE:	*pBuf ^=  mask; break;
		}
		if (h<mod) return; // fast exit if we're done here!
		h -= mod;
		pBuf += SSD1306_LCDWIDTH;
	}
	// write solid bytes while we can - effectively doing 8 rows at a time
	if (h >= 8) {
		// separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
		if (color == INVERSE) {
			do {
				*pBuf = ~(*pBuf);
				// adjust the buffer forward 8 rows worth of data
				pBuf += SSD1306_LCDWIDTH;
				// adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
				h -= 8;
			} while(h >= 8);
		} else {
			// store a local value to work with
			register uint8_t val = (color == WHITE) ? 255 : 0;
			do {
				// write our value in
				*pBuf = val;
				// adjust the buffer forward 8 rows worth of data
				pBuf += SSD1306_LCDWIDTH;
				// adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
				h -= 8;
			} while(h >= 8);
		}
	}
	// now do the final partial byte, if necessary
	if (h) {
		mod = h & 7;
		// this time we want to mask the low bits of the byte, vs the high bits we did above
		// register uint8_t mask = (1 << mod) - 1;
		// note - lookup table results in a nearly 10% performance improvement in fill* functions
		static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
		register uint8_t mask = postmask[mod];
		switch(color) {
			case WHITE:		*pBuf |=  mask; break;
			case BLACK:		*pBuf &= ~mask; break;
			case INVERSE:	*pBuf ^=  mask; break;
		}
	}
}


void oled_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	for (int16_t i=x; i<x+w; i++) oled_drawFastVLine(i, y, h, color);
}


void oled_drawPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x < 0) || (x >= SSD1306_LCDWIDTH) || (y < 0) || (y >= SSD1306_LCDHEIGHT)) return;
	switch(color) {
		case WHITE:		oled_buffer[x+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
		case BLACK:		oled_buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break;
		case INVERSE:	oled_buffer[x+ (y/8)*SSD1306_LCDWIDTH] ^=  (1 << (y&7)); break;
	}
}


uint8_t oled_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {
	if ((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT) || ((x + 6 * size - 1) < 0) || ((y + 8 * size - 1) < 0)) return 1;
	// Char bitmap = 5 columns
	for (int8_t i=0; i<5; i++ ) {
		uint8_t line = pgm_read_byte(&oled_font[c * 5 + i]);
		for (int8_t j=0; j<8; j++, line >>= 1) {
			if (line & 1) {
				if (size == 1)
					oled_drawPixel(x+i, y+j, color);
				else
					oled_fillRect(x+i*size, y+j*size, size, size, color);
			} else
				if(bg != color) {
					if(size == 1)
						oled_drawPixel(x+i, y+j, bg);
					else
						oled_fillRect(x+i*size, y+j*size, size, size, bg);
				}
		}
	}
	if (bg != color) { // If opaque, draw vertical line for last column
		if (size == 1)
			oled_drawFastVLine(x+5, y, 8, bg);
		else
			oled_fillRect(x+5*size, y, size, 8*size, bg);
	}
	return 0;
}


uint8_t oled_write(uint8_t c) {
	if (c == '\r') return 0; // Ignore carriage returns (but return 0, all is fine to continue)
	if (c == '\n') {
		oled_cursor_x = 0;
		oled_cursor_y += oled_text_size * 8;
	}
	uint8_t result = oled_drawChar(oled_cursor_x, oled_cursor_y, c, oled_text_color, oled_text_bgcolor, oled_text_size);
	oled_cursor_x += oled_text_size * 6; // if not OK, still increase cursor position..
	return result;
}


// print a string (str), at coordinates (x,y), with size (s), in colors (c,bg)
void oled_print(int16_t x, int16_t y, uint8_t s, uint16_t c, uint16_t bg, char *str) {
	oled_setCursor(x,y);
	oled_setTextSize(s);
	oled_setTextColors(c, bg);
	int len = strlen(str);
	for (int ch=0; ch<len; ch++)
		if (oled_write(str[ch]) != 0) break; // if off screen, then stop..
}


// y<0 = up
// y>0 = down
// To scroll all lines up (for the current textsize) use: y=-oled_text_size
void oled_scroll(int16_t y) {
	if (y == 0) return; // nothing to scroll..
	uint16_t r0 = 0, r1 = r0 + SSD1306_LCDWIDTH, r2 = r1 + SSD1306_LCDWIDTH, r3 = r2 + SSD1306_LCDWIDTH;
	for (r0=0; r0<SSD1306_LCDWIDTH;) {
		uint32_t col = (uint32_t)oled_buffer[r3] << 24 | (uint32_t)oled_buffer[r2] << 16 | (uint32_t)oled_buffer[r1] << 8 | (uint32_t)oled_buffer[r0];
		if (y<0) col >>= -y; else col <<= y;
		oled_buffer[r3++] = (col >> 24) & 0xFF;
		oled_buffer[r2++] = (col >> 16) & 0xFF;
		oled_buffer[r1++] = (col >> 8) & 0xFF;
		oled_buffer[r0++] = col & 0xFF;
	}
}


// scroll 1 line up
// s is the textsize
void oled_scrollLine(int16_t s) {
	if (s == 0) return; // nothing to scroll..
	if (s*8 >= SSD1306_LCDHEIGHT) { oled_cls(); return; } // completely off screen..
	uint16_t from = s * SSD1306_LCDWIDTH;
	uint16_t to = 0;
	uint16_t copyN = (4 - s) * SSD1306_LCDWIDTH;
	uint16_t emptyN = from;
	while(to < copyN) oled_buffer[to++] = oled_buffer[from++];
	memset(&oled_buffer[to], 0, emptyN);
}


// scroll and print a line of text
void oled_println(uint8_t s, uint16_t c, uint16_t bg, char *str) {
/*
	// (pixel)scroll softly
	for (uint8_t pixel=0; pixel<s*8; pixel++) {
		oled_scroll(-1); // softest
		oled_print(0, SSD1306_LCDHEIGHT-pixel, s, c, bg, str); // scroll text into view (instead of empty space)
		oled_display();
	}
*/
/*	// (pixel)scroll a line at once
	int8_t pixels = -s * 8;
	oled_scroll(pixels); // scroll a full lineheight in at once
	oled_print(0, SSD1306_LCDHEIGHT+pixels, s, c, bg, str); // scroll text into view (instead of empty space)
*/
	// (line)scroll
	oled_scrollLine(s); // faster scroll method
	oled_print(0, (4-s)*8, s, c, bg, str);
	oled_display();
}


// scroll up a line first (always fontsize 2), then print logo & text on lowest line..
void oled_printlnLogo(uint8_t logoID, char *str) {
	oled_scrollLine(2); // faster scroll method
	// the 16x16 logo is always on the lowest 2 lines
	for (uint8_t y=0; y<2; y++) {
		uint16_t yo = (y+2) * 8;
		uint8_t yl = y * 16;
		for (uint8_t x=0; x<16; x++) {
			uint8_t col;
			switch(logoID) {
				case 5:
					col = pgm_read_byte(&logoAppleHomekit[yl+x]);
					break;
				case 6:
					col = pgm_read_byte(&logoFHEM[yl+x]);
					break;
				case 7:
					col = pgm_read_byte(&logoHeater[yl+x]);
					break;
				default:
					return;
			}
			for (uint8_t bit=0; bit<8; bit++) {
				if (col & 1)
					oled_drawPixel(x, yo+bit, WHITE);
				else
					oled_drawPixel(x, yo+bit, BLACK);
				col >>= 1;
			}
		}
	}
	// print the text
	oled_print(16,16, 2, 1,0, str); // logo is 16 pixels wide, text always fontsize 2 white on black
	oled_display();
}


/**
 * This routine turns off the I2C bus and clears it
 * on return SCA and SCL pins are tri-state inputs.
 * You need to call Wire.begin() after this to re-enable I2C
 * This routine does NOT use the Wire library at all.
 *
 * returns 0 if bus cleared
 *         1 if SCL held low.
 *         2 if SDA held low by slave clock stretch for > 2sec
 *         3 if SDA held low after 20 clocks.
 */
 /*
int I2C_ClearBus() {
#if defined(TWCR) && defined(TWEN)
  TWCR &= ~(_BV(TWEN)); //Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
#endif

  pinMode(SDA, INPUT_PULLUP); // Make SDA (data) and SCL (clock) pins Inputs with pullup.
  pinMode(SCL, INPUT_PULLUP);

  delay(2500);  // Wait 2.5 secs. This is strictly only necessary on the first power
  // up of the DS3231 module to allow it to initialize properly,
  // but is also assists in reliable programming of FioV3 boards as it gives the
  // IDE a chance to start uploaded the program
  // before existing sketch confuses the IDE by sending Serial data.

  boolean SCL_LOW = (digitalRead(SCL) == LOW); // Check is SCL is Low.
  if (SCL_LOW) { //If it is held low Arduno cannot become the I2C master. 
    return 1; //I2C bus error. Could not clear SCL clock line held low
  }

  boolean SDA_LOW = (digitalRead(SDA) == LOW);  // vi. Check SDA input.
  int clockCount = 20; // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) { //  vii. If SDA is Low,
    clockCount--;
  // Note: I2C bus is open collector so do NOT drive SCL or SDA high.
    pinMode(SCL, INPUT); // release SCL pullup so that when made output it will be LOW
    pinMode(SCL, OUTPUT); // then clock SCL Low
    delayMicroseconds(10); //  for >5uS
    pinMode(SCL, INPUT); // release SCL LOW
    pinMode(SCL, INPUT_PULLUP); // turn on pullup resistors again
    // do not force high as slave may be holding it low for clock stretching.
    delayMicroseconds(10); //  for >5uS
    // The >5uS is so that even the slowest I2C devices are handled.
    SCL_LOW = (digitalRead(SCL) == LOW); // Check if SCL is Low.
    int counter = 20;
    while (SCL_LOW && (counter > 0)) {  //  loop waiting for SCL to become High only wait 2sec.
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }
    if (SCL_LOW) { // still low after 2 sec error
      return 2; // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
    }
    SDA_LOW = (digitalRead(SDA) == LOW); //   and check SDA input again and loop
  }
  if (SDA_LOW) { // still low
    return 3; // I2C bus error. Could not clear. SDA data line held low
  }

  // else pull SDA line low for Start or Repeated Start
  pinMode(SDA, INPUT); // remove pullup.
  pinMode(SDA, OUTPUT);  // and then make it LOW i.e. send an I2C Start or Repeated start control.
  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  /// A Repeat Start is a Start occurring after a Start with no intervening Stop.
  delayMicroseconds(10); // wait >5uS
  pinMode(SDA, INPUT); // remove output low
  pinMode(SDA, INPUT_PULLUP); // and make SDA high i.e. send I2C STOP control.
  delayMicroseconds(10); // x. wait >5uS
  pinMode(SDA, INPUT); // and reset pins as tri-state inputs which is the default state on reset
  pinMode(SCL, INPUT);
  return 0; // all ok
}
*/
/*
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  int rtn = I2C_ClearBus(); // clear the I2C bus first before calling Wire.begin()
  if (rtn != 0) {
    Serial.println(F("I2C bus error. Could not clear"));
    if (rtn == 1) {
      Serial.println(F("SCL clock line held low"));
    } else if (rtn == 2) {
      Serial.println(F("SCL clock line held low by slave clock stretch"));
    } else if (rtn == 3) {
      Serial.println(F("SDA data line held low"));
    }
  } else { // bus clear
    // re-enable Wire
    // now can start Wire Arduino master
    Wire.begin();
  }
  Serial.println("setup finished");
}
*/
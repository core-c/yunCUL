#define SSD1306_I2C_ADDRESS		0x3C
#define SSD1306_LCDWIDTH		128
#define SSD1306_LCDHEIGHT		32
#define SSD1306_LCDBYTES		(SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8)

#define SSD1306_SETLOWCOLUMN			0x00
#define SSD1306_SETHIGHCOLUMN			0x10
#define SSD1306_MEMORYMODE				0x20
#define SSD1306_COLUMNADDR				0x21
#define SSD1306_PAGEADDR				0x22
#define SSD1306_SETSTARTLINE			0x40
#define SSD1306_SETCONTRAST				0x81
#define SSD1306_CHARGEPUMP				0x8D
#define SSD1306_SEGREMAP				0xA0
#define SSD1306_DISPLAYALLON_RESUME		0xA4
#define SSD1306_DISPLAYALLON			0xA5
#define SSD1306_NORMALDISPLAY			0xA6
#define SSD1306_INVERTDISPLAY			0xA7
#define SSD1306_SETMULTIPLEX			0xA8
#define SSD1306_DISPLAYOFF				0xAE
#define SSD1306_DISPLAYON				0xAF
#define SSD1306_COMSCANINC				0xC0
#define SSD1306_COMSCANDEC				0xC8
#define SSD1306_SETDISPLAYOFFSET		0xD3
#define SSD1306_SETDISPLAYCLOCKDIV		0xD5
#define SSD1306_SETPRECHARGE			0xD9
#define SSD1306_SETCOMPINS				0xDA
#define SSD1306_SETVCOMDETECT			0xDB

#define SSD1306_EXTERNALVCC				0x1
#define SSD1306_SWITCHCAPVCC			0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL							0x2F
#define SSD1306_DEACTIVATE_SCROLL						0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA				0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL					0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL					0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL	0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL		0x2A

#define BLACK		0
#define WHITE		1
#define INVERSE		2


extern int16_t oled_cursor_x, oled_cursor_y;
extern uint16_t oled_text_color, oled_text_bgcolor;
extern uint8_t oled_text_size;


void oled_init(void); // init i2c & OLED
void oled_command(uint8_t c);
void oled_display(void); // show what is in the buffer
void oled_cls(void); // clear screen
void oled_setCursor(int16_t x, int16_t y);
void oled_setTextColor(uint16_t c);
void oled_setTextColors(uint16_t c, uint16_t bg);
void oled_setTextSize(uint8_t s);
void oled_drawFastVLine(int16_t x, int16_t __y, int16_t __h, uint16_t color);
void oled_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void oled_drawPixel(int16_t x, int16_t y, uint16_t color);
uint8_t oled_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
uint8_t oled_write(uint8_t c); // write one character to the screen
void oled_print(int16_t x, int16_t y, uint8_t s, uint16_t c, uint16_t bg, char *str);
void oled_softScroll(int16_t y); // y<0 = up, y>0 = down.  To scroll all lines up (for the current textsize) use: y=-oled_text_size
void oled_println(uint8_t s, uint16_t c, uint16_t bg, char *str); // scroll up a line first, then print text on lowest line..

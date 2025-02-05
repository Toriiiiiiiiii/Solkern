#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <string.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25

enum _vga_col {
    VGA_BLACK         = 0,
	VGA_BLUE          = 1,
	VGA_GREEN         = 2,
	VGA_CYAN          = 3,
	VGA_RED           = 4,
	VGA_MAGENTA       = 5,
	VGA_BROWN         = 6,
	VGA_LIGHT_GREY    = 7,
	VGA_DARK_GREY     = 8,
	VGA_LIGHT_BLUE    = 9,
	VGA_LIGHT_GREEN   = 10,
	VGA_LIGHT_CYAN    = 11,
	VGA_LIGHT_RED     = 12,
	VGA_LIGHT_MAGENTA = 13,
	VGA_LIGHT_BROWN   = 14,
	VGA_WHITE         = 15,
};

void vga_init();
void vga_putc(char c);
void vga_putc_noad(char c);
void vga_goto(unsigned int x, unsigned int y);
void vga_setcol(unsigned int x, unsigned int y, unsigned char fore, unsigned char back);
void vga_setchr(unsigned int x, unsigned int y, char c);
void vga_puts(const char *s);
void vga_scroll(unsigned int amount);
void vga_clearline(unsigned int y);
void vga_hexout(unsigned int val);
void vga_movecursor(unsigned int x, unsigned int y);
void vga_setcursor(uint8_t cursor_start, uint8_t cursor_end);
void vga_updatepos();
void vga_backspace();

uint8_t vga_col(uint8_t fore, uint8_t back);
void vga_setccol(uint8_t col);

#endif // VGA_H

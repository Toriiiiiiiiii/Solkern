#include <vga.h>
#include <pic.h>

// Basic variables needed for the VGA driver
static uint8_t *VGA_BASE = (uint8_t *)0xb8000;
static unsigned int vga_cursor_x;
static unsigned int vga_cursor_y;

// Initialise VGA display
void vga_init() {
    vga_goto(0, 0);
    vga_setcursor(0, 15);

    for(uint32_t x = 0; x < VGA_WIDTH; ++x) {
        for(uint32_t y = 0; y < VGA_HEIGHT; ++y) {
            vga_setcol(x, y, VGA_WHITE, VGA_BLACK);
            vga_setchr(x, y, '\0');
        }
    }  
}

void vga_setchr(unsigned int x, unsigned int y, char c) {
    VGA_BASE[(y*2) * VGA_WIDTH + (x*2)] = c;
}

// Output single character to VGA display
void vga_putc(char c) {
    if(c == '\n') {
        vga_cursor_x = 0;
        vga_cursor_y++;
        goto checkline;
    }

    VGA_BASE[(vga_cursor_y*2) * VGA_WIDTH + (vga_cursor_x*2)] = c;

    vga_cursor_x++;
    if(vga_cursor_x >= VGA_WIDTH) {
        vga_cursor_x = 0;
        vga_cursor_y++;
    }

checkline:
    if(vga_cursor_y >= VGA_HEIGHT) {
        vga_cursor_y = VGA_HEIGHT - 1;
        vga_scroll(1);
    }
}

void vga_putc_noad(char c) {
    VGA_BASE[(vga_cursor_y*2) * VGA_WIDTH + (vga_cursor_x*2)] = c;
}

// Set the cursor X and Y position
void vga_goto(unsigned int x, unsigned int y) {
    vga_cursor_x = (x >= VGA_WIDTH)? VGA_WIDTH - 1 : x;
    vga_cursor_y = (y >= VGA_HEIGHT)? VGA_HEIGHT - 1 : y;

    vga_movecursor(x, y);
}

// Set the colour of a character cell on screen
void vga_setcol(unsigned int x, unsigned int y, unsigned char fore, unsigned char back) {
    unsigned char val = fore | back << 4;
    unsigned int  offset = (y*2*VGA_WIDTH + x*2) + 1;

    VGA_BASE[offset] = val;
}

// Output a string to the display
void vga_puts(const char *s) {
    unsigned long index = 0;
    while(s[index]) {
        vga_putc(s[index]);
        index++;
    }

    vga_movecursor(vga_cursor_x, vga_cursor_y);
}

// Scroll vga display by <n> lines
void vga_scroll(unsigned int n) {
    for(int i = 0; i < n; ++i) {
        memcpy(VGA_BASE, VGA_BASE + (VGA_WIDTH*2), (VGA_WIDTH*2)*(VGA_HEIGHT-1));
        vga_clearline(VGA_HEIGHT-1);
    }
}

void vga_clearline(unsigned int y) {
    unsigned int tx = vga_cursor_x;
    unsigned int ty = vga_cursor_y;
    vga_goto(0, y);

    for(int x = 0; x < VGA_WIDTH-1; ++x) {
        vga_putc(' ');
        vga_setcol(x, y, VGA_WHITE, VGA_BLACK);
    }
    vga_putc_noad(' ');
    vga_setcol(VGA_WIDTH-1, VGA_HEIGHT-1, VGA_WHITE, VGA_BLACK);

    vga_goto(tx, ty);
}

void vga_hexout(unsigned int val) {
    char buf[11] = "0x00000000\0";
    char hexmap[16] = "0123456789abcdef";

    unsigned int strptr = 9;
    while(val > 0) {
        char digit = hexmap[val % 16];
        val /= 16;

        buf[strptr] = digit;
        strptr--;
    }

    vga_puts(buf);
}

void vga_movecursor(unsigned int x, unsigned int y) {
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void vga_setcursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void vga_updatepos() {
    vga_movecursor(vga_cursor_x, vga_cursor_y);
}

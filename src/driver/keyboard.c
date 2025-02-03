#include <keyboard.h>
#include <pic.h>
#include <vga.h>
#include <stdbool.h>

#define KEY_SHIFT 42

static bool shift_pressed = false;

unsigned char kb_noshift[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e',
    'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b',
    'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0,
    0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

unsigned char kb_shift[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*',
    '9', '0', '-', '=', '\b', '\t', 'Q', 'W', 'E',
    'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    ':', '\"', '~', 0, '\|', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0,
    0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_handler(regs_t* regs) {
    unsigned char scancode = inb(0x60);

    if(scancode & 0x80) {
        if((scancode & 0b01111111) == KEY_SHIFT) shift_pressed = false; 
    } else {
        if((scancode & 0b01111111) == KEY_SHIFT) { 
            shift_pressed = true;
        }
        else {
            vga_putc( shift_pressed? kb_shift[scancode] : kb_noshift[scancode] );
            vga_updatepos();
        }
    }
}

void install_keyboard_driver() { 
    irq_install_handler(1, keyboard_handler);
}

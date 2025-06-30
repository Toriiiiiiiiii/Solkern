#include "../../include/keyboard.h"
#include "../../include/pic.h"
#include "../../include/vga.h"
#include "../../include/pipe.h"
#include <stdbool.h>

#define KEY_SHIFT 0x2a
#define KEY_BCKSP 0x0e
#define KEYBUF_SIZE 256

static bool shift_pressed = false;
static char keybuf[256] = {0};
static unsigned int keybuf_front = 0;
static unsigned int keybuf_back = 0;

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
        if((scancode & 0x7f) == KEY_SHIFT) shift_pressed = false; 
    } else {
        if((scancode & 0x7f) == KEY_SHIFT) { 
            shift_pressed = true;
        } else if((scancode & 0x7f) == KEY_BCKSP) {
            keybuf[keybuf_back] = '\b';
            keybuf_back++;
        } else {
            keybuf[keybuf_back] = shift_pressed ? kb_shift[scancode] : kb_noshift[scancode];
            keybuf_back++;
        }
    }

    if(keybuf_back >= KEYBUF_SIZE) keybuf_back = 0;
}

int keyb_write(uint32_t n, void* data) {
    return 0;
}

int keyb_read(uint32_t n, void* buf) {
    if(!buf) return 0;
    if(keybuf_back == keybuf_front) return 0;

    *(char*)buf = keybuf[keybuf_front];
    keybuf_front++;

    if(keybuf_front >= KEYBUF_SIZE) keybuf_front = 0;
    return 1;
}

void install_keyboard_driver() { 
    int desc = mkpipe("kb", keyb_write, keyb_read);
    irq_install_handler(1, keyboard_handler);
}


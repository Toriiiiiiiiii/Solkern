#include <idt.h>
#include <vga.h>
#include <pic.h>
#include <keyboard.h>

void _kmain(void) {
    idt_init();
    isrs_install();
    pic_install();
    vga_init();
    install_keyboard_driver();

    vga_puts("Hello, World!\n");
    while(1) {}
}

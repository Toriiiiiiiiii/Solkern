#include <idt.h>
#include <vga.h>
#include <pic.h>

void _kmain(void) {
    pic_init();
    idt_init();
    vga_init();

    vga_puts("Hello, World!\n");
}

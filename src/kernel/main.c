#include <idt.h>
#include <vga.h>

void _kmain(void) {
    idt_init();
    vga_init();

    __asm__ volatile("div %ah");
}

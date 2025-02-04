#include <idt.h>
#include <vga.h>
#include <pic.h>
#include <keyboard.h>
#include <serial.h>

void _kmain(void) {
    vga_init();
    vga_puts("SOLKERN V0.1\n");

    vga_puts("Setting up Interrupt Descriptor Table.....\n");
    idt_init();
    vga_puts("OK!\n");

    vga_puts("Initializing Interrupt Service Routines.....\n");
    isrs_install();
    vga_puts("OK!\n");

    vga_puts("Initializing Interrupt Request Vectors.....\n");
    pic_install();
    vga_puts("OK!\n");

    vga_puts("Loading basic Keyboard Driver.....\n");
    install_keyboard_driver();
    vga_puts("OK!\n");

    vga_puts("Initializing Serial Driver.....\n");
    if(serial_install()) {
        vga_puts("ERROR!\n");
    } else {
        vga_puts("OK!\n");
        serial_sendstr("Solkern v0.1 Serial Terminal\r\n");
    }

    while(1) {}
}

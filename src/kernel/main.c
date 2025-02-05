#include <idt.h>
#include <vga.h>
#include <pic.h>
#include <keyboard.h>
#include <serial.h>

void _kmain() {
    vga_init();
    vga_puts("SOLKERN V0.1\n");

    __asm__ volatile("cli");
    vga_puts(" - Initial Boot Sequence\n");
    vga_puts("   - Setting up Interrupt Descriptor Table...\n");
    idt_init();

    vga_puts("   - Initializing Interrupt Service Routines...\n");
    isrs_install();

    vga_puts("   - Initializing Interrupt Request Vectors...\n");
    pic_install();
    __asm__ volatile("sti");

    vga_puts("   - Loading basic Keyboard Driver...\n");
    install_keyboard_driver();

    vga_puts("   - Initializing Serial Driver...\n");
    if(serial_install()) {
        vga_puts("     - ERROR\n");
    } else {
        serial_sendstr("Solkern v0.1 Serial Terminal\r\n");
    }

    while(1) {
        __asm__ volatile("hlt");
    }
}

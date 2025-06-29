#include "../../include/idt.h"
#include "../../include/vga.h"
#include "../../include/pic.h"
#include "../../include/keyboard.h"
#include "../../include/serial.h"
#include "../../include/multiboot.h"
#include "../../include/drives.h"
#include "../../include/liballoc.h"
#include "../../include/mmu.h"

void _kmain(uint32_t mb_magic, mb_info_t *mb_info) {
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

    vga_puts("   - Enabling paging...\n");
    setupPageDirectory();

    __asm__ volatile("sti");

    // Read Multiboot Information
    vga_puts("   - Reading Multiboot Information...\n");
    if(mb_magic != MULTIBOOT_MAGIC) {
        vga_puts("     - ERROR: Multiboot Verification Failed.\n");
        while(1) __asm__ volatile("hlt");
    }

    doParseTags(mb_info);

    vga_puts("   - Loading basic Keyboard Driver...\n");
    install_keyboard_driver();

    vga_puts("   - Initializing Serial Driver...\n");
    if(serial_install()) {
        vga_puts("     - ERROR\n");
    } else {
        serial_sendstr("Solkern v0.1 Serial Terminal\r\n");
    }

    vga_puts("   - INFO: Successfully initialized kernel from ");
    vga_puts(bootloader_name.string);
    vga_puts("!\n");

    while(1) {
        __asm__ volatile("hlt");
    }
}

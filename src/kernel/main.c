#include "../../include/idt.h"
#include "../../include/vga.h"
#include "../../include/pic.h"
#include "../../include/keyboard.h"
#include "../../include/serial.h"
#include "../../include/multiboot.h"

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
    __asm__ volatile("sti");

    // Read Multiboot Information
    vga_puts("   - Reading Multiboot Information...\n");
    if(mb_magic != MULTIBOOT_MAGIC) {
        vga_puts("     - ERROR: Multiboot Verification Failed.\n");
        while(1) __asm__ volatile("hlt");
    }

    // Read Memory Size Information
    if(mb_info->flags & MB_FLAG_MEMINFO) {
        vga_puts("     - INFO: Lower memory size: ");
        vga_hexout(mb_info->mem_lower);
        vga_puts(" KB\n");
        vga_puts("     - INFO: Upper memory size: ");
        vga_hexout(mb_info->mem_upper);
        vga_puts(" KB\n");
    } else {
        vga_puts("     - INFO: No memory information provided.\n");
    }

    if(mb_info->flags & MB_FLAG_BOOTDEV) {
        vga_puts("     - INFO: Boot Device = ");
        vga_hexout((mb_info->boot_device & 0xff000000) >> 24);
        vga_puts("\n");

        vga_puts("     - INFO: Boot Partition Number = ");
        vga_hexout((mb_info->boot_device & 0x00ff0000) >> 16);
        vga_puts("\n");
    }

    // Read bootloader name
    if(mb_info->flags & MB_FLAG_BOOTLDRNAME) {
        vga_puts("     - INFO: Bootloader Name: ");
        vga_puts((const char*)mb_info->bootloader_name);
        vga_puts("\n");
    } else {
        vga_puts("     - INFO: No bootloader name was provided.\n");
    }

    // Read framebuffer information
    if(mb_info->flags & MB_FLAG_FRAMEBUFFER) {
        vga_puts("     - INFO: Framebuffer Size: ");
        vga_hexout(mb_info->framebuffer_width);
        vga_puts(" by ");
        vga_hexout(mb_info->framebuffer_height);
        vga_puts(" pixels\n");
    } else {
        vga_puts("     - INFO: No framebuffer information provided by bootloader.\n");
    }

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

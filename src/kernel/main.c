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

    vga_puts("     - INFO: Flags = ");
    vga_nout(mb_info->flags, 2);
    vga_puts("\n");

    // Read Memory Size Information
    if(mb_info->flags & MB_FLAG_MEMINFO) {
        vga_puts("     - INFO: Lower memory size: ");
        vga_nout(mb_info->mem_lower, 10);
        vga_puts(" KB\n");
        vga_puts("     - INFO: Upper memory size: ");
        vga_nout(mb_info->mem_upper, 10);
        vga_puts(" KB\n");
    } else {
        vga_puts("     - INFO: No memory information provided.\n");
    }

    if(mb_info->flags & MB_FLAG_BOOTDEV) {
        vga_puts("     - INFO: Boot Device = $");
        vga_nout((mb_info->boot_device & 0xff000000) >> 24, 16);
        vga_puts("\n");

        vga_puts("     - INFO: Boot Partition Number = $");
        vga_nout((mb_info->boot_device & 0x00ff0000) >> 16, 16);
        vga_puts("\n");
    }

    if(mb_info->flags & MB_FLAG_CMDLINE) {
        vga_puts("     - INFO: Command Line: ");
        vga_puts((const char*)mb_info->cmdline);
        vga_puts("\n");
    } else {
        vga_puts("     - INFO: No command line provided.\n");
    }

    // Read all drives
    if(mb_info->flags & MB_FLAG_DRIVES) {
        vga_puts("     - INFO: Enumerating drives\n");
        void* addr = (void*)mb_info->drives_addr;
        void* end  = addr + mb_info->drives_length;

        while(addr < end) {
            uint32_t size = *(uint32_t*)addr;

            void* drv_base = addr;
            void* drv_end = addr + size;

            drive_info_t drv = {0};

            drv.size = size;
            drv.drive_number = *(uint8_t*)(addr + 4);
            drv.drive_mode = *(uint8_t*)(addr + 5);
            drv.drive_cylinders = *(uint8_t*)(addr + 6);
            drv.drive_heads = *(uint8_t*)(addr + 8);
            drv.drive_sectors = *(uint8_t*)(addr + 9);
            drv.drive_ports = (uint16_t*)malloc(size - 10);

            vga_puts("       Drive $");
            vga_nout(drv.drive_number, 16);
            vga_puts("\n");
            
            addr += 10;

            uint8_t port = 0;
            while(addr < drv_end) {
                drv.drive_ports[port] = *(uint16_t*)(addr);
                port++;
                addr += 2;
            }

            drv_register(drv.drive_number, drv);
        }
    } else {
        vga_puts("     - INFO: No drives provided.\n");
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
        vga_nout(mb_info->framebuffer_width, 10);
        vga_puts(" by ");
        vga_nout(mb_info->framebuffer_height, 10);
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

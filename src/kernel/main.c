#include "../../include/idt.h"
#include "../../include/vga.h"
#include "../../include/pic.h"
#include "../../include/keyboard.h"
#include "../../include/serial.h"
#include "../../include/multiboot.h"
#include "../../include/drives.h"
#include "../../include/mmu.h"
#include "../../include/flanterm.h"
#include "../../include/flanterm_backends/fb.h"
#include "../../include/liballoc.h"

void _kmain(uint32_t mb_magic, mb_info_t *mb_info) {
    serial_puts("SOLKERN V0.1\r\n");

    __asm__ volatile("cli");
    serial_puts(" - Initial Boot Sequence\r\n");
    serial_puts("   - Setting up Interrupt Descriptor Table...\r\n");
    idt_init();

    serial_puts("   - Initializing Interrupt Service Routines...\r\n");
    isrs_install();

    serial_puts("   - Initializing Interrupt Request Vectors...\r\n");
    pic_install();

    serial_puts("   - Enabling paging...\r\n");
    setupPageDirectory();

    __asm__ volatile("sti");

    serial_puts("   - Initializing Serial Driver...\r\n");
    if(serial_install()) {
        vga_puts("     - ERROR\n");
    }

    // Read Multiboot Information
    serial_puts("   - Reading Multiboot Information...\r\n");
    if(mb_magic != MULTIBOOT_MAGIC) {
        serial_puts("     - ERROR: Multiboot Verification Failed.\r\n");
        while(1) __asm__ volatile("hlt");

    }


    doParseTags(mb_info);

    serial_puts("   - Loading basic Keyboard Driver...\r\n");
    install_keyboard_driver();


//    struct flanterm_context *ft_ctx = flanterm_fb_init(
//            NULL, NULL,
//            (uint32_t*)fb_info.framebuffer_addr, fb_info.framebuffer_width, 
//            fb_info.framebuffer_height, fb_info.framebuffer_pitch,
//            fb_info.color_info[1], fb_info.color_info[0],
//            fb_info.color_info[3], fb_info.color_info[2],
//            fb_info.color_info[5], fb_info.color_info[4],
//            NULL, NULL, NULL,
//            NULL, NULL, NULL,
//            NULL, NULL,
//            0, 0, 1, 0, 0, 0
//    );
//
//    char msg[] = "Hello, World!\n";
//    flanterm_write(ft_ctx, msg, sizeof(msg));

    uint32_t* fb = (uint32_t*)fb_info.framebuffer_addr;
    *fb = 0xffffffff;

    while(1) {
        __asm__ volatile("hlt");
    }
}

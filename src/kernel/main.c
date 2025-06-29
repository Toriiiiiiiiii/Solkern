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

    vga_puts("   - Initializing Serial Driver...\n");
    if(serial_install()) {
        vga_puts("     - ERROR\n");
    } else {
        serial_puts("Solkern v0.1 Serial Terminal\r\n");
    }

    doParseTags(mb_info);

    vga_puts("   - Loading basic Keyboard Driver...\n");
    install_keyboard_driver();


    struct flanterm_context *ft_ctx = flanterm_fb_init(
            NULL, NULL,
            (uint32_t*)fb_info.framebuffer_addr, fb_info.framebuffer_width, 
            fb_info.framebuffer_height, fb_info.framebuffer_pitch,
            fb_info.color_info[1], fb_info.color_info[0],
            fb_info.color_info[3], fb_info.color_info[2],
            fb_info.color_info[5], fb_info.color_info[4],
            NULL, NULL, NULL,
            NULL, NULL, NULL,
            NULL, NULL,
            0, 0, 1, 0, 0, 0
    );

    char msg[] = "Hello, World!\n";
    flanterm_write(ft_ctx, msg, sizeof(msg));

    uint32_t* fb = (uint32_t*)fb_info.framebuffer_addr;
    fb[0] = 0xffffffff;

    while(1) {
        __asm__ volatile("hlt");
    }
}

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
#include "../../include/ftm.h"
#include "../../include/string.h"

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

    initFSTree();
    //serialPrintFSTree();

    serial_puts("   - Loading basic Keyboard Driver...\r\n");
    install_keyboard_driver();

    __asm__ volatile("cli");
    initTable(1);
    for(int i = 0; i < (fb_info.framebuffer_pitch * fb_info.framebuffer_height) / 4096; ++i) {
        setTableAddr(1, i, fb_info.framebuffer_addr + 4096*i);
    }

    for(int table = 2; table < 10; ++table) {
        for(int i = 0; i < 1024; ++i) {
            setTableAddr(table, i, 1024*4096 + 4096*(table-2) + 4096*i);
        }
    }
    __asm__ volatile("sti");

    serial_puts("   - Creating Flanterm Pipe...\r\n");
    if(ftm_install()) {
        serial_puts("     - ERROR: Could not initialize flanterm.\r\n");
        while(1) __asm__ volatile("hlt");
    }

    printFSTree();
    fputs(1, "\n");

    int *n = malloc(sizeof(int));
    *n = 10;

    char buf[10] = {0};
    utos(*n, 10, buf);
    fputs(1, buf);

    fputs(1, "\n");

    *n = 12;
    utos(*n, 10, buf);
    fputs(1, buf);

    char key;
    while(1) {
        int n = fread(0, 1, &key);

        if(n) fwrite(1, 1, &key);
    }
}

#include "../../../include/multiboot.h"
#include "../../../include/liballoc.h"
#include "../../../include/vga.h"

mb_tag_cmdline cmdline = {0};
mb_tag_blname  bootloader_name = {0};
mb_tag_meminfo mem_info = {0};
mb_tag_bootdev boot_device = {0};
mb_tag_vbe     vbe_info = {0};

void doParseTags(mb_info_t* ptr) {
    vga_puts("     - INFO: Total MB Info Size = ");
    vga_nout(ptr->total_size, 10);
    vga_puts(" bytes\n");
    uint8_t *tags = (uint8_t*)malloc(ptr->total_size - 8);

    uint8_t  terminated = 0;
    uint32_t tagindex = 0;
    uint8_t* tagptr = (uint8_t*)ptr->tagstart;

    while(!terminated) {
        mb_tag_basic_t tag = {
            *(uint32_t*)tagptr,
            *((uint32_t*)tagptr + 1)
        };


        switch(tag.type) {
            case MB_TAG_TERM:
                terminated = 1;
                break;

            case MB_TAG_CMDLINE:
                cmdline.type = MB_TAG_CMDLINE;
                cmdline.size = tag.size;
                cmdline.string = (char*)(tagptr)+8;

                vga_puts("     - INFO: Command Line = '");
                vga_puts(cmdline.string);
                vga_puts("'\n");

                break;

            case MB_TAG_BOOTLDR:
                bootloader_name.type = MB_TAG_BOOTLDR;
                bootloader_name.size = tag.size;
                bootloader_name.string = (char*)(tagptr)+8;

                vga_puts("     - INFO: Bootloader Name = '");
                vga_puts(bootloader_name.string);
                vga_puts("'\n");

                break;

            case MB_TAG_MEMINFO:
                mem_info.type = MB_TAG_MEMINFO;
                mem_info.size = tag.size;
                mem_info.mem_lower = *((uint32_t*)(tagptr)+2);
                mem_info.mem_upper = *((uint32_t*)(tagptr)+3);

                vga_puts("     - INFO: Memory Size = ");
                vga_nout(mem_info.mem_lower, 10);
                vga_puts(" KB (Lower), ");
                vga_nout(mem_info.mem_upper, 10);
                vga_puts(" KB (Upper), ");
                vga_nout(mem_info.mem_lower + mem_info.mem_upper, 10);
                vga_puts(" KB (Total)\n");

                break;

            case MB_TAG_BOOTDEV:
                boot_device.biosdev = *((uint32_t*)(tagptr)+2);
                boot_device.partition = *((uint32_t*)(tagptr)+3);
                boot_device.subpartition = *((uint32_t*)(tagptr)+4);

                vga_puts("     - INFO: Boot Device = 0x");
                vga_nout(boot_device.biosdev, 16);
                vga_puts(", Partition 0x");
                vga_nout(boot_device.partition, 16);
                vga_puts("\n");
                break;

            case MB_TAG_VBEINFO:
                vbe_info.type = MB_TAG_VBEINFO;
                vbe_info.size = tag.size;

                vbe_info.vbe_mode = *((uint16_t*)(tagptr)+4);
                vbe_info.vbe_interface_seg = *((uint16_t*)(tagptr)+5);
                vbe_info.vbe_interface_len = *((uint16_t*)(tagptr)+6);

                for(int i = 0; i < 512; ++i) {
                    vbe_info.vbe_control_info[i] = ((uint8_t*)(tagptr)+7)[i];
                }

                for(int i = 0; i < 256; ++i) {
                    vbe_info.vbe_control_info[i] = ((uint8_t*)(tagptr)+7+512)[i];
                }

                vga_puts("     - INFO: === VBE TABLE ===\n");
                vga_puts("       MODE =              ");
                vga_nout(vbe_info.vbe_mode, 10);
                vga_puts("\n");
                vga_puts("       INTERFACE SEGMENT = 0x");
                vga_nout(vbe_info.vbe_interface_seg, 16);
                vga_puts("\n");
                vga_puts("       INTERFACE OFFSET =  0x");
                vga_nout(vbe_info.vbe_interface_off, 16);
                vga_puts("\n");
                vga_puts("       INTERFACE LENGTH =  0x");
                vga_nout(vbe_info.vbe_interface_len, 16);
                vga_puts("\n");

                break;

            default:
                vga_puts("     - INFO: Ignoring unrecognised tag { ");
                vga_nout(tag.type, 10);
                vga_puts(", ");
                vga_nout(tag.size, 10);
                vga_puts(" } (0x");
                vga_nout((uint32_t)tagptr, 16);
                vga_puts(")\n");
                break;
        }

        tagptr += ((tag.size + 7) & ~7);
    }
}

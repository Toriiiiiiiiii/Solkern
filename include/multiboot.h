#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_MAGIC 0x2badb002

#define MB_FLAG_MEMINFO     1 << 0
#define MB_FLAG_BOOTDEV     1 << 1
#define MB_FLAG_CMDLINE     1 << 2
#define MB_FLAG_MODS        1 << 3
#define MB_FLAG_SYMS        1 << 4 || 1 << 5
#define MB_FLAG_MMAP        1 << 6
#define MB_FLAG_DRIVES      1 << 7
#define MB_FLAG_CONFIGTBL   1 << 8
#define MB_FLAG_BOOTLDRNAME 1 << 9
#define MB_FLAG_APMTBL      1 << 10
#define MB_FLAG_VBE         1 << 11
#define MB_FLAG_FRAMEBUFFER 1 << 12

typedef struct {
    uint32_t flags;

    uint32_t mem_lower;
    uint32_t mem_upper;

    uint32_t boot_device;
    uint32_t cmdline;

    uint32_t mods_count;
    uint32_t mods_addr;

    uint32_t syms[4];

    uint32_t mmap_length;
    uint32_t mmap_addr;

    uint32_t drives_length;
    uint32_t drives_addr;

    uint32_t config_table;
    uint32_t bootloader_name;

    uint32_t apm_table;

    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint32_t color_info;
} mb_info_t;

#endif

#ifndef MB_TAGS_H
#define MB_TAGS_H

#include <stdint.h>

typedef enum {
    MB_TAG_TERM    = 0,
    MB_TAG_CMDLINE = 1,
    MB_TAG_BOOTLDR = 2,
    MB_TAG_MODULES = 3,
    MB_TAG_MEMINFO = 4,
    MB_TAG_BOOTDEV = 5,
    MB_TAG_MEMMAP  = 6,
    MB_TAG_VBEINFO = 7,
    MB_TAG_FBINFO  = 8,
    MB_TAG_ELFSYMS = 9,
    MB_TAG_APMTBL  = 10,
    MB_TAG_EFITBL3 = 11,
    MB_TAG_EFITBL6 = 12,
    MB_TAG_SMBIOS  = 13,
    MB_TAG_ACPIOLD = 14,
    MB_TAG_ACPINEW = 15,
    MB_TAG_NETWORK = 16,
    MB_TAG_EFIMMAP = 17
} mb_tag_type_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t mem_lower;
    uint32_t mem_upper;
} mb_tag_meminfo;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t biosdev;
    uint32_t partition;
    uint32_t subpartition;
} mb_tag_bootdev;

typedef struct {
    uint32_t type;
    uint32_t size;
    char* string;
} mb_tag_cmdline;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t mod_start;
    uint32_t mod_end;
    char* string;
} mb_tag_modules;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint16_t num;
    uint16_t entsize;
    uint16_t shndx;
    uint16_t reserved;
    void* hdrs;
} mb_tag_elfsyms;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    void* entries;
} mb_tag_mmap;

typedef struct {
    uint32_t type;
    uint32_t size;
    char* string;
} mb_tag_blname;

typedef struct {
} mb_tag_apm;

typedef struct {
    uint32_t type;
    uint32_t size;

    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
    uint8_t  vbe_control_info[512];
    uint8_t  vbe_mode_info[256];
} mb_tag_vbe;

typedef struct {
    uint32_t type;
    uint32_t size;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint16_t framebuffer_type;
    uint8_t reserved;

    uint8_t color_info[6];
} mb_tag_framebuf;

#endif // MB_TAGS_H

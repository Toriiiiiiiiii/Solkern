#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "mb_tags.h"
#include <stdint.h>

#define MULTIBOOT_MAGIC 0x36d76289

typedef struct {
    uint32_t type;
    uint32_t size;
} mb_tag_basic_t;

typedef struct {
    uint32_t total_size;
    uint32_t reserved;
    mb_tag_basic_t tagstart[0];
} mb_info_t;

void doParseTags(mb_info_t* ptr);

extern mb_tag_cmdline cmdline;
extern mb_tag_blname  bootloader_name;
extern mb_tag_meminfo mem_info;
extern mb_tag_bootdev boot_device;
extern mb_tag_vbe     vbe_info;

#endif

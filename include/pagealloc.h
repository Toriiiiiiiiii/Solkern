#ifndef PAGEALLOC_H
#define PAGEALLOC_H

#include <stdint.h>

void* kalloc_alloc(int n);
void  kalloc_free(void* ptr, int n);

void  set_frame_status(int frameid, uint8_t s);

#endif

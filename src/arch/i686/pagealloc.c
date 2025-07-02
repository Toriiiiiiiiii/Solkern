#include "../../../include/pagealloc.h"
#include <stdint.h>

#define NPAGES 1024

extern uint32_t end_kernel;

uint8_t framemap[NPAGES / 8] = {0};

uint8_t get_frame_status(int frameid) {
    uint32_t index = frameid / 8;
    uint32_t shift = frameid % 8;
    uint8_t  byte  = framemap[index];

    return (byte & (1 << shift)) >> shift;
}

void set_frame_status(int frameid, uint8_t s) {
    uint32_t index = frameid / 8;
    uint32_t shift = frameid % 8;

    framemap[index] |= (s & 1) << shift;
}

void* kalloc_alloc(int n) {
    uint32_t first_frame = (end_kernel) / 4096 + 1;
    uint32_t index = 0;
    while(index < NPAGES) {
        
        uint8_t success = 1;
        for(int i = 0; i < n; ++i) {
            if(get_frame_status(index + i)) { 
                success = 0;
                break;
            }
        }

        if(success) {
            for(int i = 0; i < n; ++i) {
                set_frame_status(index + i, 1);
            }

            return (void*)(first_frame + (index * 4096));
        }

        index++;
    }

    return (void*)0;
}

void kalloc_free(void* ptr, int n) {
    uint32_t first_frame = (end_kernel + 4096) / 4096;
    uint32_t index = ((uint32_t)ptr - first_frame) / 4096;

    for(int i = 0; i < n/8; ++i) {
        for(int j = 0; j < 8; ++j) {
            set_frame_status(index+8*i+j, 0);
        }
    }
}

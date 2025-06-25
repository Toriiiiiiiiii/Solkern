#include <stdint.h>
#include "../../include/liballoc.h"

#define N_PAGES 1024
#define PAGE_SIZE 4096

static uint8_t heap[N_PAGES * PAGE_SIZE];
static uint8_t page_status[N_PAGES] = {0};

int liballoc_lock() {
    __asm__ volatile("cli");
    return 0;
}

int liballoc_unlock() {
    __asm__ volatile("sti");
    return 0;
}

void* liballoc_alloc(int n) {
    // Find first free run of `n` pages
    for(int i = 0; i < N_PAGES - n; ++i) {
        uint8_t failed = 0;
        for(int j = 0; j < n; ++j) {
            if(page_status[i+j]) {
                failed = 1;
                break;
            }
        }

        if(failed) continue;
        return &(heap[i * PAGE_SIZE]);
    }

    return NULL;
}

int liballoc_free(void* ptr, int n) {
    uint32_t page = (uint32_t)ptr / PAGE_SIZE;
    for(int i = 0; i < n; i++) {
        page_status[page + i] = 0; 
    }

    return 0;
}

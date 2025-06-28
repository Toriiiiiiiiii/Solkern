#include <stdint.h>
#include "../../include/liballoc.h"
#include "../../include/pagealloc.h"

int liballoc_lock() {
    __asm__ volatile("cli");
    return 0;
}

int liballoc_unlock() {
    __asm__ volatile("sti");
    return 0;
}

void* liballoc_alloc(int n) {
    return kalloc_alloc(n);
}

int liballoc_free(void* ptr, int n) {
    kalloc_free(ptr, n);
    return 0;
}

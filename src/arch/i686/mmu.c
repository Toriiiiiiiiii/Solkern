#include "../../../include/mmu.h"
#include "../../../include/liballoc.h"
#include "../../../include/pagealloc.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t page_table[1024] __attribute__((aligned(4096)));
static uint32_t page_table_list_size = 1;
static uint32_t page_table_list_cap  = 2;

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();
extern void flushTLB();

void loadFirstTable();

uint8_t setupPageDirectory() {
    for(int i = 0; i < 1024; ++i) {
        page_directory[i] = 0x2;
    }

    loadFirstTable();

    for(int i = 0; i < page_table_list_size; ++i) {
        page_directory[i] = ((uint32_t)page_table) | 3;
    }

    loadPageDirectory((unsigned int*)page_directory);
    enablePaging();
    return 0;
}

void loadFirstTable() {
    for(int i = 0; i < 1024; ++i) {
        page_table[i] = (i * 0x1000) | 3;
    }
}

void initTable(unsigned int index) {
    uint32_t *table __attribute__((aligned(4096))) = (uint32_t*)kalloc_alloc(1);
    for(int i = 0; i < 1024; ++i) {
        table[i] = 0;
    }

    page_directory[index] = ((uint32_t)table) | 3;
    flushTLB();
}

void setTableAddr(unsigned int tblIndex, unsigned int page, uint32_t addr) {
    uint32_t* table = (uint32_t*)(page_directory[tblIndex] & ~(uint32_t)(3));
    table[page] = addr | 3;
    flushTLB();
}

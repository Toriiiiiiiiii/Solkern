#include "../../../include/mmu.h"
#include "../../../include/liballoc.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static page_table_t* page_tables;
static uint32_t page_table_list_size = 1;
static uint32_t page_table_list_cap  = 2;

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

void loadFirstTable();

uint8_t setupPageDirectory() {
    for(int i = 0; i < 1024; ++i) {
        page_directory[i] = 0x2;
    }

    page_tables = (page_table_t*)malloc(page_table_list_cap * sizeof(page_table_t));
    loadFirstTable();

    for(int i = 0; i < page_table_list_size; ++i) {
        page_directory[i] = ((uint32_t)page_tables[i].data) | 3;
    }

    loadPageDirectory((unsigned int*)page_directory);
    enablePaging();
    return 0;
}

void loadFirstTable() {
    for(int i = 0; i < 1024; ++i) {
        page_tables[0].data[i] = (i * 0x1000) | 3;
    }
}

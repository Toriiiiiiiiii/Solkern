#ifndef MMU_H
#define MMU_H

#include <stdint.h>

typedef struct {
    uint32_t data[1024];
} page_table_t __attribute__((aligned(4096)));

uint8_t setupPageDirectory(void);

#endif // MMU_H

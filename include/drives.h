#ifndef DRIVES_H
#define DRIVES_H

#include <stdint.h>

#define MAX_DRIVES 256

typedef struct {
    uint32_t  size;
    uint8_t   drive_number;
    uint8_t   drive_mode;
    uint16_t  drive_cylinders;
    uint8_t   drive_heads;
    uint8_t   drive_sectors;
    uint16_t* drive_ports;
} drive_info_t;

void drv_register(uint8_t id, drive_info_t drv);

#endif

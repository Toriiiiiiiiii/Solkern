#include "../../include/drives.h"
static drive_info_t drives[MAX_DRIVES];

void drv_register(uint8_t id, drive_info_t drv) {
    drives[id] = drv;
}

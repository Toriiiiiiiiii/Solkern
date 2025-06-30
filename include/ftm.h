#ifndef FTM_H
#define FTM_H

#include "fs.h"
#include "pipe.h"
#include <stdint.h>

// Flanterm Driver
extern int ftm_descriptor;

int ftm_install();
int ftm_write(uint32_t n, void* data);
int ftm_read(uint32_t n, void* buf);

#endif // FTM_H

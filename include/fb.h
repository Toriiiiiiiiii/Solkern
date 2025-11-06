#ifndef FB_H
#define FB_H

#include "fs.h"
#include "pipe.h"
#include <stdint.h>

int fb_install();
int fb_write(uint32_t n, void* data);
int fb_read(uint32_t n, void* buf);

typedef struct {
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
	uint32_t* pixels;
} fb_region_t;

fb_region_t makeRegion(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

#endif // FB_H

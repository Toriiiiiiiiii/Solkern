#include "../../include/fb.h"
#include "../../include/io.h"
#include "../../include/multiboot.h"
#include "../../include/liballoc.h"

int fb_descriptor = 0;

int fb_install()
{
	fb_descriptor = mkpipe("dev/fb", fb_write, fb_read);
	if(fb_descriptor == -1) return 1;
	return 0;
}

int fb_write(uint32_t n, void* data) 
{
	uint8_t* fb = (uint8_t*)(1024 * 4096);
	fb_region_t reg = *(fb_region_t*)data;

	uint32_t regionX = reg.x;
	uint32_t regionY = reg.y;
	uint32_t regionW = reg.w;
	uint32_t regionH = reg.h;
	uint32_t* region = reg.pixels;

	//fprintf(1, "Region Coords: %d %d\n", regionX, regionY);

	if(regionX >= fb_info.framebuffer_width || regionY >= fb_info.framebuffer_height)
		return 0;

	if(regionX + regionW >= fb_info.framebuffer_width || regionY + regionH >= fb_info.framebuffer_height)
		return 0;

	for(int y = regionY; y < regionY + regionH; ++y) {
		for(int x = regionX; x < regionX + regionW; ++x) {
			uint32_t rY = y - regionY;
			uint32_t rX = x - regionX;
			((uint32_t*)(fb))[y * fb_info.framebuffer_width + x] = region[rY * regionW + rX];
		}
	}

	return regionW * regionH;
}

// TODO: Give ability to read from framebuffer.
int fb_read(uint32_t n, void* buf) 
{
	return 0;
}


fb_region_t makeRegion(uint32_t x, uint32_t y, uint32_t w, uint32_t h) 
{
	return (fb_region_t){
		.x = x,
		.y = y,
		.w = w,
		.h = h,
		.pixels = (uint32_t*)malloc(sizeof(uint32_t) * w * h)
	};
}

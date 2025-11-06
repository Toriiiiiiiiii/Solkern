#include "../../include/spirograph.h"
#include "../../include/fb.h"
#include "../../mathlib/src/mathlib_pure.h"
#include "../../include/fs.h"
#include "../../include/io.h"

void doSpirograph(double outerRadius, double innerRadius, double penOffset, double maxT, double tStep)
{
	fb_region_t region = makeRegion(1024/2, 768/2, 1024/2 - 1, 768/2 - 1);
	fprintf(1, "%x\n", (uint32_t)region.pixels + region.w * region.h * 4);
	fprintf(1, "%x\n", 1024 * 4096);

	for(int i = 0; i < region.w * region.h; ++i) {
		region.pixels[i] = 0x0000;
		if(region.pixels[i] != 0) {
			fprintf(1, "%x   %x\n", (uint32_t)region.pixels + i, region.pixels[i]);
		}
	}

	for(double t = 0; t < maxT; t += tStep) {
		double x = (outerRadius - innerRadius) * ml_cos(t) 
			     + penOffset * ml_cos(((outerRadius - innerRadius)/innerRadius) * t);

		double y = (outerRadius - innerRadius) * ml_sin(t)
			     - penOffset * ml_sin(((outerRadius - innerRadius)/innerRadius) * t);

		region.pixels[(int)(y + region.h / 2) * region.w + (int)(x + region.w / 2)] = 0xffffffff;
	}

	fwrite(2, sizeof(region), &region);
	free(region.pixels);
}

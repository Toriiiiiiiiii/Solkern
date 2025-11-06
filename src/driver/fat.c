#include "../../include/fat.h"

fat_header_t readFatHeader(int drive) 
{
	char bootsector[512] = {0};
	ata_readSectors(1, 0, drive, bootsector);

	fat_header_t* ptr = (fat_header_t*)bootsector;
	return *ptr;
}

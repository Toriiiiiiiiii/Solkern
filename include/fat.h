#ifndef FAT_H
#define FAT_H

#include "ata.h"
#include <stdint.h>

typedef struct {
	char magic[3];
	char ident[8];
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t nreserved;
	uint8_t nfats;
	uint16_t nroot_entries;
	uint16_t total_sectors;
	uint8_t media_descriptor;
	uint16_t sectors_per_fat;
	uint16_t sectors_per_track;
	uint16_t nheads;
	uint32_t hidden_sectors;
	uint32_t large_sector_count;
} fat_header_t;

fat_header_t readFatHeader(int drive);

#endif // FAT_H

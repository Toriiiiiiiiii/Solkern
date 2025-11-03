#include "../../include/ata.h"
#include "../../include/pic.h"
#include "../../include/io.h"

int checkDrive(int driveID) {
	outb(DRIVESELECT, driveID);

	for(int i = SECTORCOUNT; i <= LBA_HI; ++i) {
		outb(i, 0);
	}

	outb(COMMANDIO, CMD_IDENTIFY);
	int status = inb(COMMANDIO);
	if(!status) return 0;

	while(status & 0x80) {
		status = inb(COMMANDIO);
	}

	if(inb(LBA_MID) || inb(LBA_HI)) return 0;
	return 1;
}

int checkForSlaveDrive() {
	return checkDrive(0xb0);
}

int checkForMasterDrive() {
	return checkDrive(0xa0);
}

void ata_reset() {
	outb(CONTROL_REG, 0b00000100);

	volatile int counter = 0;
	for(volatile int i = 0; i < 10; ++i) {
		counter++;
	}

	outb(CONTROL_REG, 0b00000100);
}

int ata_readSectors(int nSectors, int lba, int drive, void* buf) {
	outb(DRIVESELECT, 0xE0 | drive << 4 | ((lba >> 24) & 0x0f));
	outb(0x1f1, 0x00);
	outb(SECTORCOUNT, nSectors);
	outb(LBA_LO, (lba & 0xff));
	outb(LBA_MID, ((lba >> 8) & 0xff));
	outb(LBA_HI, ((lba >> 16) & 0xff));
	outb(COMMANDIO, CMD_READ);

	int sectorsRead = 0;
	int bufIndex = 0;
	while(sectorsRead < nSectors) {
		// Poll status register
		uint8_t status = inb(COMMANDIO);
		while(status & 0x80 || !(status & 0x08)) {
			status = inb(COMMANDIO);
		}

		if(status & 1 || status & (1 << 5)) return 1;

		for(int i = 0; i < 256; ++i) {
			*((uint16_t*)buf + bufIndex) = inw(0x1f0);
			bufIndex += 1;
		}

		sectorsRead++;
	}

	return 0;
}

#ifndef ATA_H
#define ATA_H

#define SECTORCOUNT 0x1f2
#define LBA_LO 0x1f3
#define LBA_MID 0x1f4
#define LBA_HI 0x1f5
#define DRIVESELECT 0x1f6
#define COMMANDIO 0x1f7

#define CMD_IDENTIFY 0xec
#define CMD_READ 0x20

#define DRIVE_SLAVE 0xF0
#define DRIVE_MASTER 0xE0
#define CONTROL_REG 0x3f6

int checkForSlaveDrive();
int checkForMasterDrive();

int ata_readSectors(int nSectors, int lba, int drive, void* buf);
void ata_reset();

#endif // ATA_H

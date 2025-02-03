#ifndef PIC_H
#define PIC_H

#include <stdint.h>

void pic_init();
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait(void);

#endif // PIC_H

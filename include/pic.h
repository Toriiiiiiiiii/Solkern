#ifndef PIC_H
#define PIC_H

#include <stdint.h>
#include "idt.h"

void pic_init();
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait(void);

void pic_eoi(int irq);
void pic_install();
void irq_install_handler(unsigned int irq, void (*handler)(regs_t* r));

#endif // PIC_H

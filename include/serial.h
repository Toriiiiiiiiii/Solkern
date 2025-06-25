#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

#include "pic.h"

#define PORT 0x3f8

int serial_install();
void serial_handler(regs_t *regs);

void serial_sendchr(char c);
void serial_sendstr(char *str);

#endif // SERIAL_DRIVER_H

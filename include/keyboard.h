#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "idt.h"

void keyboard_handler(regs_t* regs);
void install_keyboard_driver();

#endif // KEYBOARD_H

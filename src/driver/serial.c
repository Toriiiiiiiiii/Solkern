#include <serial.h>
#include <idt.h>
#include <pic.h>
#include <vga.h>

int serial_install() {
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x80);
    outb(PORT + 0, 0x03);
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x03);
    outb(PORT + 2, 0xC7);
    outb(PORT + 4, 0x0B);
    outb(PORT + 4, 0x1E);
    outb(PORT + 0, 0xAE);

    if(inb(PORT + 0) != 0xAE) {
       return 1;
    }

    outb(PORT + 4, 0x0F);
    irq_install_handler(4, serial_handler); 
    outb(PORT + 1, 0x01);
    return 0;
}

void serial_handler(regs_t *regs) {
    char serial = inb(PORT);
	serial_sendchr(serial);
}

void serial_sendchr(char c) {
    if(c == 13) {
        outb(PORT, '\r');
        outb(PORT, '\n');
    }
    else outb(PORT, c);
}

void serial_sendstr(char *s) {
    unsigned long index = 0;
    while(s[index]) {
        serial_sendchr(s[index]);
        index++;
    }
}

#include "../../include/serial.h"
#include "../../include/idt.h"
#include "../../include/pic.h"
#include "../../include/liballoc.h"

// Install the serial IRQ and set up
// serial port for transmission.
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

    irq_install_handler(4, serial_handler); // Install IRQ handler 
    outb(PORT + 1, 0x01); // Enable serial IRQ
    return 0;
}

// Handle incoming IRQs from serial port
void serial_handler(regs_t *regs) {
    char serial = inb(PORT);

    // Echo character back to serial port
	serial_sendchr(serial);
}

// Send character to serial port
void serial_sendchr(char c) {
    // Send newline
    if(c == 13) {
        outb(PORT, '\r');
        outb(PORT, '\n');
    }

    // Output character
    else outb(PORT, c);
}

// Send string to serial port
void serial_puts(char *s) {
    unsigned long index = 0;
    while(s[index]) {
        serial_sendchr(s[index]);
        index++;
    }
}

void serial_nout(unsigned int val, unsigned int base) {
    uint32_t n = 0;
    uint32_t temp = val;

    if(val == 0) {
        serial_puts("0");
        return;
    }
    else {
        while(temp > 0) {
            n++;
            temp /= base;
        }
    }

    char* buf = (char*)malloc(n + 1);
    buf[n] = '\0';

    char charmap[32] = "0123456789abcdefghijklmnopqrstu";

    unsigned int strptr = n-1;
    while(val > 0) {
        char digit = charmap[val % base];
        val /= base;

        buf[strptr] = digit;
        strptr--;
    }

    serial_puts(buf);
    free(buf);
}

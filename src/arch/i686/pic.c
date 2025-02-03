#include <pic.h>
#include <idt.h>
#include <vga.h>

static void *handlers[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

extern void irq_stub_0();
extern void irq_stub_1();
extern void irq_stub_2();
extern void irq_stub_3();
extern void irq_stub_4();
extern void irq_stub_5();
extern void irq_stub_6();
extern void irq_stub_7();
extern void irq_stub_8();
extern void irq_stub_9();
extern void irq_stub_10();
extern void irq_stub_11();
extern void irq_stub_12();
extern void irq_stub_13();
extern void irq_stub_14();
extern void irq_stub_15();

void pic_init() {
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void pic_install() {
    __asm__ volatile("cli");
    pic_init();

    idt_set_descriptor(32, irq_stub_0, 0x8E);
    idt_set_descriptor(33, irq_stub_1, 0x8E);
    idt_set_descriptor(34, irq_stub_2, 0x8E);
    idt_set_descriptor(35, irq_stub_3, 0x8E);
    idt_set_descriptor(36, irq_stub_4, 0x8E);
    idt_set_descriptor(37, irq_stub_5, 0x8E);
    idt_set_descriptor(38, irq_stub_6, 0x8E);
    idt_set_descriptor(39, irq_stub_7, 0x8E);
    idt_set_descriptor(40, irq_stub_8, 0x8E);
    idt_set_descriptor(41, irq_stub_9, 0x8E);
    idt_set_descriptor(42, irq_stub_10, 0x8E);
    idt_set_descriptor(43, irq_stub_11, 0x8E);
    idt_set_descriptor(44, irq_stub_12, 0x8E);
    idt_set_descriptor(45, irq_stub_13, 0x8E);
    idt_set_descriptor(46, irq_stub_14, 0x8E);
    idt_set_descriptor(47, irq_stub_15, 0x8E);

    __asm__ volatile("sti");
}

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

void io_wait(void) {
    outb(0x80, 0);
}

void pic_eoi(int n) {
    if (n >= 12) {
		outb(0xA0, 0x20);
	}

	outb(0x20, 0x20);
}

void irq_install_handler(unsigned int irq, void (*handler)(regs_t *r)) {
    handlers[irq] = (void *)handler;
}

void irq_handler(regs_t* regs) {
    __asm__ volatile("cli");
    void (*handler)(regs_t *r) = handlers[regs->interrupt-32];

    if(handler) handler(regs);
    pic_eoi(regs->interrupt-32);

    __asm__ volatile("sti");
}

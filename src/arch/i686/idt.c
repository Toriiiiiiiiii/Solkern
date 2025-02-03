#include <idt.h>
#include <vga.h>

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance
static idtr_t idtr;

static bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];

const char *exception_msgs[32] = {
    "Division by Zero Error",
    "Debug Exception",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow Error",
    "Bound Range Exceeded"
};

void exception_handler(exception_regs_t* regs) {
    vga_puts("Unhandled Exception: ");
    vga_puts(exception_msgs[regs->interrupt]);
    vga_puts("\n\nRegister Dump:");

    vga_puts("\n  |-> eip - ");
    vga_hexout(regs->eip);
    vga_puts("\n  |-> eax - ");
    vga_hexout(regs->eax);
    vga_puts("\n  |-> ecx - ");
    vga_hexout(regs->ecx);
    vga_puts("\n  |-> edx - ");
    vga_hexout(regs->edx);
    vga_puts("\n  |-> ebx - ");
    vga_hexout(regs->ebx);
    vga_puts("\n  |-> ebp - ");
    vga_hexout(regs->ebp);
    vga_puts("\n  |-> esp - ");
    vga_hexout(regs->esp);
    vga_puts("\n  |-> esi - ");
    vga_hexout(regs->esi);
    vga_puts("\n  |-> edi - ");
    vga_hexout(regs->edi);
    vga_puts("\n\n");

    vga_puts("Kernel Panic: System halted.\n");


    __asm__ volatile("cli; hlt");
    while(1) {}
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}

#include <idt.h>
#include <vga.h>

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance
static idtr_t idtr;

extern void* isr_stub_table[];

const char *exception_msgs[32] = {
    "Division by Zero Error",
    "Debug Exception",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow Error",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "RESESRVED",
    "x87 Floating Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "RESERVED",
    "FPU Error Interrupt"
};

void exception_handler(regs_t* regs) {
    vga_setccol( vga_col(VGA_LIGHT_BROWN, VGA_BLACK) );
    vga_puts("\n---\n");
    vga_puts("--- Unhandled Exception: ");
    vga_puts(exception_msgs[regs->interrupt]);
    vga_puts(" (Error Code: ");
    vga_hexout(regs->error);
    vga_puts(")");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_puts("\n\nRegister Dump:");

    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("\n  [eip: ");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_hexout(regs->eip);
    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("] [ebp: ");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_hexout(regs->ebp);
    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("] [esp: ");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_hexout(regs->esp);
    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("]");

    vga_puts("\n  [eax: ");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_hexout(regs->eax);
    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("] [ecx: ");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_hexout(regs->ecx);
    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("] [edx: ");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_hexout(regs->edx);
    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("]");

    vga_puts("\n  [ebx: ");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_hexout(regs->ebx);
    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("] [esi: ");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_hexout(regs->esi);
    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("] [edi: ");
    vga_setccol( vga_col(VGA_LIGHT_CYAN, VGA_BLACK) );
    vga_hexout(regs->edi);
    vga_setccol( vga_col(VGA_CYAN, VGA_BLACK) );
    vga_puts("]\n\n");

    vga_setccol( vga_col(VGA_LIGHT_RED, VGA_BLACK) );
    vga_puts("--- Kernel Panic: System halted.\n");

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

    memset((char*)idt, 0, sizeof(idt_entry_t) * 256);

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
}

void isrs_install() {
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }
}

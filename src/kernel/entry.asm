MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
MBFLAGS  equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

section .bss
align 16

stack_bottom: resb 16384
stack_top:

section .text
global _start:function (_start.end - _start)
extern _kmain

_start:
    mov     esp, stack_top
    cli
    push    ebx
    push    eax

    lgdt    [_gdt_desc]
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    jmp     0x08:.setcs
.setcs:
    pop     ebx
    push    ebx
    call    _kmain
    cli
    hlt
.done:
    jmp .done
.end:


_gdt:
_gdt_null:
    dq 0
_gdt_code:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 0b10011010
    db 0b11001111
    db 0x00
_gdt_data:
    dw 0x0FFFF
    dw 0

    db 0
    db 0b10010010
    db 0b11001111
    db 0
_gdt_end:
_gdt_desc:
    dw $ - _gdt - 1
    dd _gdt


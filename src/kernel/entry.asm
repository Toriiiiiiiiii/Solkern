MAGIC equ 0xE85250D6
ARCH  equ 0
LEN   equ mbheader_end - mbheader_start
CHECK equ 0x100000000 - (MAGIC + ARCH + LEN)

section .multiboot
align 8
mbheader_start:
	dd MAGIC
	dd ARCH
    dd LEN
    dd CHECK

    ; FRAMEBUFFER TAG
    dw 5
    dw 0
    dd 20
    dd 640
    dd 400
    dd 24
    dw 0

    dd 0
    dd 8
mbheader_end:

section .bss
align 16

stack_bottom: resb 65535
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


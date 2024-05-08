[BITS 32]

global _start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

extern kernel_main

; The start of the kernel. The bootloader hands control over to this
; code after loading the kernel image at address 0x100000.
_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable the A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    call kernel_main 

    sti ; Start interrupts after they were disabled in the bootloader.

    jmp $ ; Loop forever

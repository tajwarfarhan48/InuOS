[BITS 32]

global _start
global int00h
global int20h
global int21h
global no_int

extern kernel_main
extern int00h_handler
extern int20h_handler
extern int21h_handler
extern no_int_handler

CODE_SEG equ 0x08
DATA_SEG equ 0x10

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

    sti ; Enable interrupts

    jmp $

int00h:
    cli
    pushad

    call int00h_handler

    popad
    sti

    jmp $

int20h:
    cli
    pushad 

    call int20h_handler

    popad
    sti
    iret

int21h:
    cli
    pushad

    call int21h_handler

    popad
    sti
    iret

no_int:
    cli
    pushad

    call no_int_handler

    popad
    sti
    iret

times 512-($ - $$) db 0

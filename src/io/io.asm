[BITS 32]

section .asm

global insb
global insw
global outb
global outw

extern print

insb:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov dx, [ebp+8]
    in al, dx

    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov dx, [ebp+8]
    in ax, dx

    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    xor edx, edx
    xor eax, eax
    mov dx, [ebp+8]
    mov al, [ebp+12]
    out dx, al 

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    xor edx, edx
    xor eax, eax
    mov dx, [ebp+8]
    mov ax, [ebp+12]
    out dx, ax

    pop ebp
    ret
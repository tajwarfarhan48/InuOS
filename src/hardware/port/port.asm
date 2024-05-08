[BITS 32]

section .asm

global insb
global insw
global outb
global outw

; uint8_t insb(uint16_t port);
;
; Reads a byte from the given port
insb:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov dx, [ebp+8]
    in al, dx

    pop ebp
    ret

; uint16_t insw(uint16_t port);
; 
; Reads a word (16 bytes) from the given port
insw:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov dx, [ebp+8]
    in ax, dx

    pop ebp
    ret

; void outb(uint16_t port, uint8_t val);
;
; Writes the given byte to the given port
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

; void outw(uint16_t port, uint16_t val);
; 
; Writes the given word (16 bytes) to the given port
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
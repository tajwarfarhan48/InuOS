[BITS 32]

section .asm
global idt_load

; void idt_load(struct idt_descriptor* idtr_addr);
;
; Loads the IDT descriptor into the IDTR register
idt_load:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]
    lidt[ebx]
    pop ebp
    ret
#include "idt/idt.h"
#include "config.h"
#include "memory/memory.h"

struct gate_descriptor idt[KERNEL_NUM_INTERRUPTS];
struct idt_descriptor r_idt;

void idt_init()
{
    memset(idt, 0, KERNEL_NUM_INTERRUPTS * sizeof(struct gate_descriptor));
    r_idt.size = (KERNEL_NUM_INTERRUPTS * sizeof(struct gate_descriptor)) - 1;
    r_idt.offset = (uint32_t) idt;
    idt_load(&r_idt);
}

void add_idt_entry(int interrupt_no, void* address)
{
    struct gate_descriptor* temp = idt;
    temp += interrupt_no;

    temp->offset_1 = ((uint32_t) address) & 0x0000ffff;
    temp->selector = KERNEL_CODE_SEG;
    temp->zero = 0x00;
    temp->options = 0xEE;
    temp->offset_2 = ((uint32_t) address) >> 16;
}
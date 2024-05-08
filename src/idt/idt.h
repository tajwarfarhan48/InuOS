#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include "memory/memory.h"

#define NUM_INTERRUPTS 512

struct gate_descriptor {
    uint16_t offset_1; // Bits 0 - 15 of the offset
    uint16_t selector; // Segment selector for gate
    uint8_t zero; // Reserved space
    uint8_t options; 
    uint16_t offset_2;
}__attribute__((packed));

struct idt_descriptor {
    uint16_t size;
    uint32_t offset;
}__attribute__((packed));

void idt_init();
extern void idt_load(struct idt_descriptor* idtr_addr);
void add_idt_entry(int interrupt_no, void* address);

#endif
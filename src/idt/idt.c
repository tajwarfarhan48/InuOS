#include "idt/idt.h"
#include "idt/int_handlers.h"
#include "kernel.h"
#include "memory/memory.h"

struct gate_descriptor idt[NUM_INTERRUPTS]; // Array of interrupt gates
struct idt_descriptor r_idt;

/*
 * Initializes the Interrupt Descriptor Table (IDT)
 */
void idt_init()
{
    int i;

    memset(idt, 0, NUM_INTERRUPTS * sizeof(struct gate_descriptor));
    r_idt.size = (NUM_INTERRUPTS * sizeof(struct gate_descriptor)) - 1;
    r_idt.offset = (uint32_t) idt;
    
    idt_load(&r_idt);

    // Adds a default handler for uninitialized interrupts
    for (i = 0; i < NUM_INTERRUPTS; i++)
        add_idt_entry(i, no_int_handler);
    
    add_idt_entry(0, divide_by_zero_handler);
    add_idt_entry(0x20, timer_handler);
    add_idt_entry(0x21, keyboard_press_handler);
}

/*
 * Adds a gate to the IDT.
 *
 * Input:
 *     - int interrupt_no: The IRQ number for the gate
 *     - void* address: The address of the interrupt handler
 */
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
#include <stdint.h>
#include "hardware/port/port.h"
#include "hardware/pic/pic.h"

/*
 * Remap the interrupt numbers for the master and slave PICs.
 *
 * Reasoning: In Protected Mode, the interrupt numbers for the master 
 *            PIC (0x08 - 0x0F) overlap with the reserved numbers for 
 *            Intel (upto 0x1F). Hence, this function remaps the master
 *            and slave PIC interrupt numbers from 0x20 to 0x2F (just
 *            after the reserved range.) 
 * 
 *            However, in order to  to remap the PICs, we have to reset 
 *            them, which is what this code does.
 * 
 * Input:
 *     - uint8_t master_offset: Offset (starting IRQ) for master PIC 
 *     - uint8_t slave_offset: Offset (starting IRQ) for slave PIC 
 */
void pic_remap(uint8_t master_offset, uint8_t slave_offset)
{
    uint8_t master_mask, slave_mask;

    // Save the current interrupt masks
    master_mask = insb(PIC1_DATA);
    slave_mask = insb(PIC2_DATA);

    // Give the initialization command to the PICs
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    
    // Give the vector offsets to the PICs
    outb(PIC1_DATA, master_offset);
    io_wait();
    outb(PIC2_DATA, slave_offset);
    io_wait();

    // Tell the master and slave PICs how they are wired to each other
    outb(PIC1_DATA, 0b00000100);  // Tell master PIC there is a slave at IRQ 2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 0b00000010);  // Tell slave PIC its cascade identity (0000 0010)
    io_wait();

    // Give additional information about the environment
    outb(PIC1_DATA, ICW4_8086);  // Have the PICs use 8086 mode (not 8080)
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Restore the stored interrupt masks
    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);
}
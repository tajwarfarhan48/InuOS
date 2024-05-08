#include "hardware/port/port.h"

/*
 * In older hardware, successive operations on the PIC needed 
 * to be buffered by a short wait. This write operation on an
 * unused port (0x80) simulates such a wait.
 */
inline void io_wait()
{
    outb(0x80, 0);
}
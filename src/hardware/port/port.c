#include "hardware/port/port.h"

inline void io_wait()
{
    outb(0x80, 0);
}
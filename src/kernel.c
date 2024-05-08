#include <stdint.h>
#include <stddef.h>
#include "kernel.h"
#include "idt/idt.h"
#include "hardware/pic/pic.h"
#include "hardware/ps2/ps2.h"
#include "io/vga/vga.h"

void kernel_main()
{
    terminal_initialize();
    print("Welcome!\n\n");

    print("Initializing IDT...\n");
    idt_init();

    print("Setting up PIC...\n");
    pic_remap(0x20, 0x28);

    print("Initializing PS/2 controller...\n");
    initialise_ps2();

    print("Done!\n");
}
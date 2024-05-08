# Intro

This is an operating system for the x86 architecture with the following features:
  - A PC-AT-compatible legacy bootloader 
  - Virtual Memory in Protected Mode using the Global Descriptor Table
  - Hardware initialization code for a 8259 PIC and the PS2 keyboard controller
  - Interrupt handlers and VGA Memory Initialization to read keyboard scan codes and print ASCII characters to the terminal

# Compatibility

Being written for legacy hardware, the OS assumes an MBR partitioning scheme and BIOS firmware. As of now, there is no GPT paritioning scheme (dual MBR) or EFI System Partition for UEFI support.

# Boot sequence

After the BIOS loads the bootloader code (src/boot/boot.asm) at physical address 0x7C00 and hands control over to it, it performs the following actions:

1. Load the Global Descriptor Table (GDT)
2. Switches CPU to Protected Mode (32-bit mode)
3. Loads the kernel from the disk image and loads it at address 0x100000 and jumps to it (src/kernel.asm)

# Source Code Tour
- **boot:** Contains boot-related files and the bootloader
- **hardware/pic:** Code for setting up and remapping the Programmable Interrupt Controller (PIC)
- **hardware/port:** Port communications code (reading from/writing to ports)
- **hardware/ps2:** Setting up the PS2 controller to be able to receive keyboard input
- **hardware/idt:** Initializing the Interrupt Descriptor Table (IDT). Also contains handler code (for processing incoming Interrupt Requests (IRQs)) and service routines (for performing the actions requested by a given IRQ)
- **io/vga:** Code for printing characters to the terminal using the VGA Buffer (at address 0xB800.)
- **memory:** Memory setting and management code

# Disk Image Format
As of now, the kernel is compiled as a plain binary image instead of an ELF or COFF executable. The OS currently does not contain a method for parsing these file types.

During compilation, the bootloader is compiled separately as a 512-byte binary image (to serve as the Master Boot Record) and prepended to the kernel image using dd (see Makefile).
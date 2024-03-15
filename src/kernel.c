#include <stdint.h>
#include <stddef.h>
#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"

extern void int00h();
extern void int20h();
extern void int21h();
extern void no_int();

char ScancodeToASCII[2][128] = {
  { 0,0,0,0,0,27,27,0,         0,0,0,0,0,9,96,0,         0,0,0,0,0,113,49,0,       0,0,122,115,97,119,50,0,    // w/o SHIFT or ALT(GR)
    0,99,120,100,101,52,51,0,0,32,118,102,116,114,53,0,0,110,98,104,103,121,54,0,0,0,109,106,117,55,56,0,
    0,44,107,105,111,48,57,0,0,46,47,108,59,112,45,0,  0,0,39,0,91,61,0,0,     0,0,10,93,0,124,0,0,
    0,60,0,0,0,0,8,0,        0,49,0,52,55,0,0,0,          0,46,50,53,54,56,27,0,         0,43,51,45,42,57,0,0  },
  { 0,0,0,0,0,0,0,0,         0,0,0,0,0,0,126,0,        0,0,0,0,0,81,33,0,        0,0,90,83,65,87,64,35,       // with SHIFT
    0,67,88,68,69,36,35,0,    0,32,86,70,84,82,37,0,     0,78,66,72,71,89,94,0,    0,0,77,74,85,38,42,0,
    0,60,75,73,79,41,40,0,   0,62,63,76,58,80,95,0,   0,0,34,0,123,43,0,0,      0,0,0,125,0,92,0,0,
    0,62,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,          0,0,0,0,0,0,0,0,          0,0,0,0,0,0,0,0  } };

size_t terminal_row = 0, terminal_col = 0;
uint16_t* terminalPtr = 0;

static void terminal_putchar( size_t row, size_t col, char c, char color ) {
    terminalPtr[( row * VGA_WIDTH ) + col] = ( color << 8 ) | c;
}

static void terminal_writechar( char c, char color ) {
    if ( c == '\n' ) {
        terminal_col = 0;
        terminal_row++;
        return;
    }

    terminal_putchar( terminal_row, terminal_col, c, color );
    
    terminal_col++;

    if ( terminal_col >= VGA_WIDTH ) {
        terminal_col = 0;
        terminal_row++;
    }
}

static void terminal_initialize() {
    size_t i, j;
    terminalPtr = ( uint16_t* ) 0xB8000;

    for ( i = 0; i < VGA_HEIGHT; i++ ) {
        for ( j = 0; j < VGA_WIDTH; j++ ) {
            terminal_putchar( i, j, ' ', 0 );
        }
    }
}

void print( char* s ) {
    while ( *s != '\0' ) terminal_writechar( *(s++), 15 );
}

void int00h_handler()
{
    print("Divide by zero error\n");
}

void int20h_handler()
{
    outb(PIC1_COMMAND, PIC_EOI);
}

void int21h_handler()
{   
    static uint8_t shiftIsActive = 0;      // state of the SHIFT key
    static uint8_t capsLockIsActive = 0;   // state of the Caps Lock key
    static uint8_t nextIsReleased = 0;     // indicating that the next key was released
    
    uint8_t scanCode = insb(0x60);

    switch (scanCode)
    {
        case 0x12: case 0x59:  // SHIFT LEFT, SHIFT RIGHT 
            shiftIsActive = !nextIsReleased; 
            nextIsReleased = 0; 
            break;

        case 0x58:  // Caps Lock
            if (!nextIsReleased) {
                capsLockIsActive = !capsLockIsActive;
            }
            nextIsReleased = 0;
            break;

        case 0xF0:  // key release indicator 
            nextIsReleased = 1; 
            break;
    
        default:  // any other key
            if (!nextIsReleased)  // is it a 'key pressed' event?
            {
                scanCode = ScancodeToASCII[shiftIsActive][scanCode & 127];

                if (capsLockIsActive && scanCode > 0x60 && scanCode < 0x7B) {
                    scanCode -= 0x20;
                }

                if (scanCode != 0x00) {
                    terminal_writechar(scanCode, 15);          
                }             
            }
            nextIsReleased = 0;
            break;
    }

    outb(PIC1_COMMAND, PIC_EOI);
}

void no_int_handler()
{
    outb(PIC1_COMMAND, PIC_EOI);
}

void kernel_main()
{
    int i;
    terminal_initialize();

    print("Initializing IDT...\n");
    idt_init();

    for (i = 0; i < KERNEL_NUM_INTERRUPTS; i++)
        add_idt_entry(i, no_int);
    
    add_idt_entry(0, int00h);
    add_idt_entry(0x20, int20h);
    add_idt_entry(0x21, int21h);

    print("Setting up PIC...\n");
    pic_remap(0x20, 0x28);

    print("Initializing PS/2 controller...\n");
    initialise_ps2();

    print("Done!\n");
}
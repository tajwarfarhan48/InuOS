#include <stdint.h>
#include "io/vga/vga.h"
#include "hardware/port/port.h"
#include "hardware/pic/pic.h"

char ScancodeToASCII[2][128] = {
  { 0,0,0,0,0,27,27,0,         0,0,0,0,0,9,96,0,         0,0,0,0,0,113,49,0,       0,0,122,115,97,119,50,0,    // w/o SHIFT or ALT(GR)
    0,99,120,100,101,52,51,0,0,32,118,102,116,114,53,0,0,110,98,104,103,121,54,0,0,0,109,106,117,55,56,0,
    0,44,107,105,111,48,57,0,0,46,47,108,59,112,45,0,  0,0,39,0,91,61,0,0,     0,0,10,93,0,124,0,0,
    0,60,0,0,0,0,8,0,        0,49,0,52,55,0,0,0,          0,46,50,53,54,56,27,0,         0,43,51,45,42,57,0,0  },
  { 0,0,0,0,0,0,0,0,         0,0,0,0,0,0,126,0,        0,0,0,0,0,81,33,0,        0,0,90,83,65,87,64,35,       // with SHIFT
    0,67,88,68,69,36,35,0,    0,32,86,70,84,82,37,0,     0,78,66,72,71,89,94,0,    0,0,77,74,85,38,42,0,
    0,60,75,73,79,41,40,0,   0,62,63,76,58,80,95,0,   0,0,34,0,123,43,0,0,      0,0,0,125,0,92,0,0,
    0,62,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,          0,0,0,0,0,0,0,0,          0,0,0,0,0,0,0,0  } };

void divide_by_zero_service_routine()
{
    print("Divide by zero error\n");
}

void keyboard_press_service_routine()
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
#include <stdint.h>
#include <stddef.h>
#include "io/vga/vga.h"

static size_t terminal_row = 0, terminal_col = 0;
static uint16_t* terminalPtr = 0;

/*
 * Put a character in the VGA character buffer.
 *
 * Input:
 *     - size_t row: Row index of the VGA buffer
 *     - size_t col: Column index of the VGA buffer
 *     - char c: The character to put
 *     - char color: The color of the character
 */
static void terminal_putchar( size_t row, size_t col, char c, char color ) {
    terminalPtr[( row * VGA_WIDTH ) + col] = ( color << 8 ) | c;
}

/*
 * Write a character to the terminal with a given color.
 *
 * Input:
 *     - char c: The character to write
 *     - char color: The color of the character
 */
static void terminal_writechar( char c, char color ) {
    if ( c == '\n' ) {  // Handle newline character
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

/*
 * Initialize the VGA memory buffer to all 0s
 */
void terminal_initialize() {
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

void putc(char c) {
    terminal_writechar(c, 15);
}
#include <stdint.h>
#include <stddef.h>
#include "io/vga/vga.h"

static size_t terminal_row = 0, terminal_col = 0;
static uint16_t* terminalPtr = 0;

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
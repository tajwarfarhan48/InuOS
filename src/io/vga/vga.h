#ifndef VGA_H
#define VGA_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void terminal_initialize();
void print(char* s);
void putc(char c);

#endif
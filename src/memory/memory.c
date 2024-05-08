#include "memory/memory.h"

/*
 * Initialize a region of memory to a given value.
 *
 * Input:
 *     - void* ptr: The starting address of the to-be-initialized region
 *     - int c: The value that the memory bytes are set to
 *     - size_t size: The number of bytes to set/initialize
 */
void* memset(void* ptr, int c, size_t size)
{
    int i;
    char* p = (char*) ptr;

    for(i = 0; i < size; i++)
        p[i] = (char) c; 

    return ptr;
}
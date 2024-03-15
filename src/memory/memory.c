#include "memory/memory.h"

void* memset(void* ptr, int c, size_t size)
{
    int i;
    char* p = (char*) ptr;

    for(i = 0; i < size; i++)
        p[i] = (char) c; 

    return ptr;
}
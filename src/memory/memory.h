#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

// Physical page allocation bitmasks
#define PHY_ALLOCATED 0x1
#define PHY_BLOCKSTART 0X2

// Constants describing the size of physical memory
#define PHY_PAGESIZE_BITS 12
#define PHY_PAGESIZE 1 << PHY_PAGESIZE_BITS
#define PHY_NUMPAGES 1 << (32 - PHY_PAGESIZE_BITS)

// Structure representing information about a page
typedef uint8_t phy_pageinfo;
extern phy_pageinfo phy_pages[PHY_NUMPAGES];

void* memset(void* ptr, int c, size_t size);
uint32_t kmalloc(uint32_t size);
uint32_t kzalloc(uint32_t size);

#endif
#include "memory/memory.h"

phy_pageinfo phy_pages[PHY_NUMPAGES];

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

static inline uint32_t phy_to_pfn(uint32_t phy_addr) {
    return phy_addr >> PHY_PAGESIZE_BITS;
}

static inline uint32_t pfn_to_phy(uint32_t pfn) {
    return pfn << PHY_PAGESIZE_BITS;
}

uint32_t kmalloc(uint32_t size) {
    uint32_t num_alloc_pages, i, x, alloc_pfn = 0;

    num_alloc_pages = size / (PHY_PAGESIZE) + ((size % (PHY_NUMPAGES) == 0) ? 0 : (PHY_PAGESIZE));
    
    // Handle 0-byte allocations
    if (num_alloc_pages == 0) goto fail;

    for (i = 0; i < PHY_NUMPAGES; i++) {
        // Empty page found
        if (!(phy_pages[i] | PHY_ALLOCATED)) {
            if (x == 0) alloc_pfn = i; // This is the starting address of the allocation
            x++;
            if (x == num_alloc_pages) break; // We have found a large enough contiguous block of memory
        }

        else x = 0; // Large enough contiguous block not found. Restart search
    }

    if (x == num_alloc_pages) {
        phy_pages[alloc_pfn] = phy_pages[alloc_pfn] | PHY_BLOCKSTART; // Mark page as start of allocated block

        // Mark just-allocated pages as already allocated 
        for (i = alloc_pfn; i < alloc_pfn + x; i++)
            phy_pages[i] = phy_pages[i] | PHY_ALLOCATED;

        return pfn_to_phy(alloc_pfn);
    }

fail:
    return (uint32_t) -1;
}

uint32_t kzalloc(uint32_t size) {
    uint32_t alloc_addr;

    if ((alloc_addr = kmalloc(size)) == -1) return -1;

    memset((void *) alloc_addr, 0, size);

    return alloc_addr;
}

void kree(uint32_t addr) {
    uint32_t pfn;

    if (addr % PHY_PAGESIZE != 0) {
        panic();
    }

    pfn = phy_to_pfn(addr);

    if (!(phy_pages[pfn] & PHY_BLOCKSTART)) {
        panic();
    }

    phy_pages[pfn] &= ~PHY_BLOCKSTART;

    while (!(phy_pages[pfn] & PHY_BLOCKSTART) && (phy_pages[pfn] & PHY_ALLOCATED)) {
        phy_pages[pfn] &= ~PHY_ALLOCATED;
        pfn++;
    }
}
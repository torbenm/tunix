#include "stdlib.h"
#include "mem.h"

extern char __free_ram[], __free_ram_end[];

paddr_t alloc_pages(uint32_t n)
{
    static paddr_t next_page = 0;
    if (!next_page)
        next_page = (paddr_t)__free_ram;
    paddr_t paddr = next_page;
    next_page += n * PAGE_SIZE;

    if (next_page > (paddr_t)__free_ram_end)
        PANIC("out of memory");

    memset((void *)paddr, 0, n * PAGE_SIZE);

    return paddr;
}
#include <shrdlib.h>

#include "mem.h"
#include "panic.h"

extern char __free_ram[], __free_ram_end[], __kernel_base[];

/*
    A full documentation can be found on Page 104 of
    the privileged instruction documentation for RISCV.

    | Virtual Address | VPN[1] (10 bits) | VPN[0] (10 bits) | Offset (12 bits) |
    |-----------------|------------------|------------------|------------------|
    |   0x1000_0000   | 0x040            | 0x00f            | 0x012            |

    Full entry -> 0x 040 00f 012
    The 22-bit physical page number is translated from the 20-bit VPN1 & VPN0 - the
    page offset is not translated. Thus we can have 2^12 byte page sizes, which
    equals 4096 bytes.
    The page offset itself will just be appended to the translated bytes, thus it
    targets the offset within the page itself.
    Since the virtual addresses cover 20 bit, we can have 2^20 ~ 1.000.000 pages per
    process, and in total 2^22 ~ 4.000.000 physical pages.

    The page table entry itself looks the following:
    | PPN[1] (12 bits) | PPN[0] (10 bits) | RSW (2 bits) | D | A | G | U | X | W | R | V |

    Exact meaning of the flags:
    V = Is entry valid
    R = Read
    W = Write
    X = Executable
    U = User Mode
    G = Gloabla Mapping
    A = Was Accessed?
    D = Dirty?
    RSW = Reserved for supervisor

    ! Important note: While we store the page table in memory,
    the TLB will cache the latest entries for performance.
*/

paddr_t alloc_pages(uint32_t n)
{
    static paddr_t next_page = 0;
    if (!next_page)
        next_page = (paddr_t)__free_ram;
    paddr_t paddr = next_page;
    next_page += n * PAGE_SIZE;

    if (next_page > (paddr_t)__free_ram_end)
        PANIC("out of memory: %d", next_page);

    memset((void *)paddr, 0, n * PAGE_SIZE);

    return paddr;
}

void map_page(uint32_t *table1, uint32_t vaddr, paddr_t paddr, uint32_t flags)
{
    /*
    table1: Reference to the in-memory pagetable
    vaddr: The virtual address we want to map
    paddr: The physical address we will map
    flags: The flags we want to set
    */

    if (!is_aligned(vaddr, PAGE_SIZE))
        PANIC("unaligned vaddr %x", vaddr);

    if (!is_aligned(paddr, PAGE_SIZE))
        PANIC("unaligned addr %x", paddr);

    // Building a two-level page table
    // Find first table index (10 bits)
    // Q: Why & 0x3ff?

    // Here we shift the bits 22 positions to the right,
    // so that we get the VPN1 bits (--> 0x040)
    // Doing & 0x3ff will return 0x040 with 10 bits (remove all other bits),
    // as 0x3ff == 1111111111 (10 bits)
    uint32_t vpn1 = (vaddr >> 22) & 0x3ff;

    // Do a table look up and see if the page is activated.
    if ((table1[vpn1] & PAGE_V) == 0)
    {
        // Table entry does not exist / is not valid, so we create one.
        // First allocate a new page for our purpose.
        uint32_t pt_paddr = alloc_pages(1);
        // We need to find the physical page number, but no the page itself.
        // Also marking the entry as valid.
        table1[vpn1] = ((pt_paddr / PAGE_SIZE) << 10) | PAGE_V;
    }

    // Set the 2nd level page table entry
    // Again, we get the VPN[0] (e.g. 0x00f) and ensure 10 bits.
    uint32_t vpn0 = (vaddr >> 12) & 0x3ff;
    // Fetch the table for the second level - and set the vpn0 entry with the rwx flags and valid.
    uint32_t *table0 = (uint32_t *)((table1[vpn1] >> 10) * PAGE_SIZE);
    table0[vpn0] = ((paddr / PAGE_SIZE) << 10) | flags | PAGE_V;
}

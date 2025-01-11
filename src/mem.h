#define SATP_SV32 (1u << 31) // single bit in the satp register which indicates "enable paging in Sv32 mode"
#define PAGE_V (1 << 0)      // "Valid" bit (entry is enabled) == 0000.0001
#define PAGE_R (1 << 1)      // Readable == 0000.0010
#define PAGE_W (1 << 2)      // Writable == 0000.0100
#define PAGE_X (1 << 3)      // Executable == 0000.1000
#define PAGE_U (1 << 4)      // User (accessible in user mode) == 0001.0000
#define PAGE_SIZE 4096

paddr_t alloc_pages(uint32_t n);

typedef struct
{
    paddr_t next_page;
} mem_state;

void mem_state_init(mem_state *state);
void map_page(uint32_t *table1, uint32_t vaddr, paddr_t paddr, uint32_t flags);
#define PAGE_SIZE 4096

paddr_t alloc_pages(uint32_t n);

typedef struct
{
    paddr_t next_page;
} mem_state;

void mem_state_init(mem_state *state);
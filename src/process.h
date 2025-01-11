#include "stdlib.h"

#define PROCS_MAX 8     // Maximum number of processes
#define PROC_UNUSED 0   // Unused process control structure
#define PROC_READY 1    // Ready process
#define PROC_RUNNING 2  // Runnable process
#define PROC_SLEEPING 3 // Sleeping/Blocked process

struct process
{
    int pid;              // Process ID
    int state;            // Process state
    vaddr_t sp;           // Stack pointer
    uint32_t *page_table; // Page Table
    uint8_t stack[8192];  // Kernel stack
};

struct process *create_process(uint32_t);
void switch_context(uint32_t *, uint32_t *);
void yield(void);
void init_process();
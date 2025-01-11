#include "common.h"

#define PROCS_MAX 8     // Maximum number of processes
#define PROC_UNUSED 0   // Unused process control structure
#define PROC_READY 1    // Ready process
#define PROC_RUNNING 2  // Runnable process
#define PROC_SLEEPING 3 // Sleeping/Blocked process
#define PROC_EXITED 4   // Exited process

#define WAIT_FOR_NONE 0
#define WAIT_FOR_INPUT 1

#define USER_BASE 0x1000000
#define SSTATUS_SPIE (1 << 5)
#define SSTATUS_SUM (1 << 18)

struct process
{
    int pid;              // Process ID
    int state;            // Process state
    uint8_t waiting_for;  // Waiting for change
    vaddr_t sp;           // Stack pointer
    uint32_t *page_table; // Page Table
    uint8_t stack[8192];  // Kernel stack
};

struct process *create_process(const void *, size_t);
void switch_context(uint32_t *, uint32_t *);
void yield(void);
void init_process();
void exit_current_process();
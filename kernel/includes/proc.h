#include "common.h"
#include "trap.h"

#define PROCS_MAX 8  // Maximum number of processes
#define THRDS_MAX 16 // Maximum number of threads

#define PROC_UNUSED 0   // Unused process control structure
#define PROC_READY 1    // Ready process
#define PROC_RUNNING 2  // Runnable process
#define PROC_SLEEPING 3 // Sleeping/Blocked process
#define PROC_EXITED 4   // Exited process

#define THRD_READY 1
#define THRD_RUNNING 2
#define THRD_SLEEPING 3
#define THRD_EXITED 4

#define USER_BASE 0x1000000
#define SSTATUS_SPIE (1 << 5)
#define SSTATUS_SUM (1 << 18)

// Saved registers for kernel context switches.
struct context
{
    uint32_t ra;
    uint32_t sp;

    // callee-saved
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
};

struct process
{
    int pid;                       // Process ID
    int state;                     // Process state
    struct context context;        // Context
    struct trap_frame *trap_frame; // Trap Frame
    uint32_t *page_table;          // Page Table
    uint8_t stack[8192];           // Kernel stack
};

struct thread
{
    int tid;                 // Thread ID
    int state;               // Thread status
    struct process *process; // Parent process
    uint64_t stack[4096];
};

struct process *create_process(const void *image, size_t image_size);
struct process *find_next_proc();
void switch_context(struct context *, struct context *);
void save_context(struct context *);

void yield(void);
void init_proc();
void exit_current_process();
int idle();

int fork(void);
struct process *alloc_proc();
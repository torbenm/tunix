#include <shrdlib.h>

#include "proc.h"
#include "mem.h"
#include "panic.h"
#include "virtio.h"
#include "csr.h"
#include "trap.h"

struct process procs[PROCS_MAX]; // All process control structures.
struct thread threads[THRDS_MAX];
struct process *current_proc; // Currently running process
struct process *idle_proc;    // Idle process

extern char __kernel_base[], __free_ram_end[];

// Okay, so this is what we need:
// - A way to fork the current process
// - A way to overwrite the current process

void init_proc()
{
    idle_proc = create_process(NULL, 0);
    idle_proc->pid = -1; // idle
    current_proc = idle_proc;

    printf("Processing initialized.\n");
}

int idle()
{
    // Check if there are any process to run...
    // TBD: Replace with threads
    int has_processes = 0;
    for (int i = 0; i < PROCS_MAX; i++)
    {
        if (procs[i].state != PROC_UNUSED && procs[i].state != PROC_EXITED && procs[i].pid > 0)
        {
            has_processes = 1;
            break;
        }
    }
    if (!has_processes)
        return 0; // Stop idling

    sleep(300000);
    printf("cpu is idle...\n");
    yield();
    return 1; // Keep idling
}

int fork(void)
{
    struct process *proc = alloc_proc();

    copy_pages(current_proc->page_table, proc->page_table, USER_BASE, MAX_VIRT_ADDR);

    // Copy context...
    memcpy(proc->stack, current_proc->stack, sizeof(current_proc->stack));
    *(proc->trap_frame) = *(current_proc->trap_frame);
    proc->trap_frame->epc += 4;
    proc->trap_frame->a0 = 0;

    return proc->pid;
}
struct process *currentproc()
{
    return current_proc;
}

struct process *find_next_proc()
{
    // Find an unused process control structure.
    struct process *proc = NULL;
    int i;
    for (i = 0; i < PROCS_MAX; i++)
    {
        if (procs[i].state == PROC_UNUSED)
        {
            proc = &procs[i];
            proc->pid = i + 1;
            break;
        }
    }

    if (!proc)
        PANIC("no free process slots");

    return proc;
}

__attribute__((naked)) void enter_usermode(void)
{
    __asm__ __volatile__(
        "csrw sstatus, %[sstatus]  \n"
        :
        : [sstatus] "r"(SSTATUS_SPIE | SSTATUS_SUM));
}

void user_entry(void)
{
    // printf("Enter user mode.");
    // printf("Leave trap on %x.", (struct trap_frame *)VIRT_TRAP_TABLE);
    leavetrap((struct trap_frame *)VIRT_TRAP_TABLE);
    enter_usermode();
}

struct process *alloc_proc()
{
    struct process *proc = find_next_proc();
    proc->state = PROC_READY;

    proc->page_table = (uint32_t *)alloc_pages(1);
    proc->trap_frame = (struct trap_frame *)alloc_pages(1);

    // Map trap table to last virtual addr
    map_page(proc->page_table, VIRT_TRAP_TABLE, (uint32_t)proc->trap_frame, PAGE_R | PAGE_W);

    // Map kernel pages. Here physical addr = virtual addr
    for (paddr_t paddr = (paddr_t)__kernel_base;
         paddr < (paddr_t)__free_ram_end; paddr += PAGE_SIZE)
        map_page(proc->page_table, paddr, paddr, PAGE_R | PAGE_W | PAGE_X);

    // Map virtio pages.  Here physical addr = virtual addr
    map_page(proc->page_table, VIRTIO_BLK_PADDR, VIRTIO_BLK_PADDR, PAGE_R | PAGE_W);

    memset(&proc->context, 0, sizeof(proc->context));
    proc->context.sp = (uint32_t)(&proc->stack[sizeof(proc->stack)]);
    proc->context.ra = (uint32_t)enter_usermode; // make this user_ret

    return proc;
}

struct process *create_process(const void *image, size_t image_size)
{

    struct process *proc = alloc_proc();

    // Map user pages.
    for (uint32_t off = 0; off < image_size; off += PAGE_SIZE)
    {
        paddr_t page = alloc_pages(1);

        // Handle the case where the data to be copied is smaller than the
        // page size.
        size_t remaining = image_size - off;
        size_t copy_size = PAGE_SIZE <= remaining ? PAGE_SIZE : remaining;

        // Fill and map the page.
        memcpy((void *)page, image + off, copy_size);
        map_page(proc->page_table, USER_BASE + off, page,
                 PAGE_U | PAGE_R | PAGE_W | PAGE_X);
    }
    proc->trap_frame->epc = USER_BASE;
    return proc;
}

void yield(void)
{
    // Search for a runnable process
    struct process *next = idle_proc;
    for (int i = 0; i < PROCS_MAX; i++)
    {
        struct process *proc = &procs[(current_proc->pid + i) % PROCS_MAX];
        // Simple round-robin scheduling
        if (proc->state == PROC_READY && proc->pid > 0)
        {
            next = proc;
            break;
        }
    }
    // If there's no runnable process other than the current one, return and continue processing
    if (next == current_proc)
        return;

    __asm__ __volatile__(
        "sfence.vma\n"
        "csrw satp, %[satp]\n" // Page table...
        "sfence.vma\n"
        :
        // Don't forget the trailing comma!
        : [satp] "r"(SATP_SV32 | ((uint32_t)next->page_table / PAGE_SIZE)));

    struct process *prev = current_proc;
    current_proc = next;
    switch_context(&prev->context, &next->context);
}

void exit_process(int pid, int status)
{
    printf("process %d exited with status %i\n", pid, status);
    // Pid is always index + 1
    procs[pid - 1].state = PROC_EXITED;
    yield();
    PANIC("unreachable");
}

void exit_current_process(int status)
{
    exit_process(current_proc->pid, status);
}

__attribute((naked)) void save_context(struct context *context)
{
    __asm__ __volatile__(
        "sw ra,  0  * 4(a0)\n" // Save callee-saved registers only
        "sw sp,  1  * 4(a0)\n" // Save callee-saved registers only
        "sw s0,  3  * 4(a0)\n"
        "sw s1,  4  * 4(a0)\n"
        "sw s2,  5  * 4(a0)\n"
        "sw s3,  6  * 4(a0)\n"
        "sw s4,  7  * 4(a0)\n"
        "sw s5,  8  * 4(a0)\n"
        "sw s6,  9  * 4(a0)\n"
        "sw s7,  10 * 4(a0)\n"
        "sw s8,  11 * 4(a0)\n"
        "sw s9,  12 * 4(a0)\n"
        "sw s10, 13 * 4(a0)\n"
        "sw s11, 14 * 4(a0)\n");
}

__attribute__((naked)) void switch_context(struct context *prev_ctxt,
                                           struct context *next_ctxt)
{
    __asm__ __volatile__(

        "sw ra,  0  * 4(a0)\n" // Save callee-saved registers only
        "sw sp,  1  * 4(a0)\n" // Save callee-saved registers only
        "sw s0,  2  * 4(a0)\n"
        "sw s1,  3  * 4(a0)\n"
        "sw s2,  4  * 4(a0)\n"
        "sw s3,  5  * 4(a0)\n"
        "sw s4,  6  * 4(a0)\n"
        "sw s5,  7  * 4(a0)\n"
        "sw s6,  8  * 4(a0)\n"
        "sw s7,  9 * 4(a0)\n"
        "sw s8,  10 * 4(a0)\n"
        "sw s9,  11 * 4(a0)\n"
        "sw s10, 12 * 4(a0)\n"
        "sw s11, 13 * 4(a0)\n"

        "lw ra,  0  * 4(a1)\n" // Restore callee-saved registers only
        "lw sp,  1  * 4(a1)\n" // Restore callee-saved registers only
        "lw s0,  2  * 4(a1)\n"
        "lw s1,  3  * 4(a1)\n"
        "lw s2,  4  * 4(a1)\n"
        "lw s3,  5  * 4(a1)\n"
        "lw s4,  6  * 4(a1)\n"
        "lw s5,  7  * 4(a1)\n"
        "lw s6,  8  * 4(a1)\n"
        "lw s7,  9  * 4(a1)\n"
        "lw s8,  10 * 4(a1)\n"
        "lw s9,  11 * 4(a1)\n"
        "lw s10, 12 * 4(a1)\n"
        "lw s11, 13 * 4(a1)\n"

        "ret\n");
}

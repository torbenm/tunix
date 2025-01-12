#include <shrdlib.h>

#include "trap.h"
#include "csr.h"
#include "panic.h"
#include "proc.h"
#include "syscall.h"
#include "mem.h"

void trap_panic(const char *mesg)
{
    uint32_t scause = READ_CSR(scause);
    uint32_t stval = READ_CSR(stval);
    uint32_t user_pc = READ_CSR(sepc);
    PANIC("\n\n%s scause=0x%x, stval=0x%x, sepc=0x%x\n", mesg, scause, stval, user_pc);
}

void leavetrap(struct trap_frame *f)
{
    WRITE_CSR(sepc, f->epc);
    trap_exit();
}

void handle_trap(struct trap_frame *f)
{
    uint32_t scause = READ_CSR(scause);
    uint32_t stval = READ_CSR(stval);
    f->epc = READ_CSR(sepc);

    switch (scause)
    {
    // Expected traps...
    case SCAUSE_TIMER_INTERRUPT:
        next_timer();
        yield();
        break;
    case SCAUSE_ECALL:
        handle_syscall(f);
        f->epc += 4; // increase program counter so that we skip to the next instruction
        break;
    // Error Handling below...
    case SCAUSE_PAGE_FAULT:
        trap_panic("Page Fault!");
        break;
    case SCAUSE_ILLEGAL_INSTRUCTION:
        trap_panic("Illegal Instruction!");
        break;
    case 7:
        trap_panic("Store/AMO access fault!");
        break;
    default:
        trap_panic("Unexpected Trap!");
        break;
    }
    leavetrap(f);
}

__attribute__((naked))
__attribute__((aligned(4))) void
trap_entry(void)
{
    __asm__ __volatile__(
        // Retrieve the kernel stack of the running process from sscratch.
        // Store a0 to cscratch so that we can reuse it for other purposes...
        "csrw sscratch, a0\n"
        // Now we can load the vappr for the trapframe here (always at the same location).
        "li a0, %[TRAPFRAME]\n"
        "sw ra,  4 * 0(a0)\n"
        "sw gp,  4 * 1(a0)\n"
        "sw tp,  4 * 2(a0)\n"
        "sw t0,  4 * 3(a0)\n"
        "sw t1,  4 * 4(a0)\n"
        "sw t2,  4 * 5(a0)\n"
        "sw t3,  4 * 6(a0)\n"
        "sw t4,  4 * 7(a0)\n"
        "sw t5,  4 * 8(a0)\n"
        "sw t6,  4 * 9(a0)\n"

        "sw a1,  4 * 11(a0)\n"
        "sw a2,  4 * 12(a0)\n"
        "sw a3,  4 * 13(a0)\n"
        "sw a4,  4 * 14(a0)\n"
        "sw a5,  4 * 15(a0)\n"
        "sw a6,  4 * 16(a0)\n"
        "sw a7,  4 * 17(a0)\n"
        "sw s0,  4 * 18(a0)\n"
        "sw s1,  4 * 19(a0)\n"
        "sw s2,  4 * 20(a0)\n"
        "sw s3,  4 * 21(a0)\n"
        "sw s4,  4 * 22(a0)\n"
        "sw s5,  4 * 23(a0)\n"
        "sw s6,  4 * 24(a0)\n"
        "sw s7,  4 * 25(a0)\n"
        "sw s8,  4 * 26(a0)\n"
        "sw s9,  4 * 27(a0)\n"
        "sw s10, 4 * 28(a0)\n"
        "sw s11, 4 * 29(a0)\n"
        "sw sp, 4 * 30(a0)\n"

        // Save original a0 in trap frame via a1
        "csrr a1, sscratch\n"
        "sw a1, 4 * 10(a0)\n"

        // // Reset the kernel stack.
        // "addi a0, sp, 4 * 31\n"
        // "csrw sscratch, a0\n"

        "call handle_trap\n"

        :
        : [TRAPFRAME] "i"(VIRT_TRAP_TABLE));
}

__attribute__((naked))
__attribute__((aligned(4))) void
trap_exit(void)
{

    __asm__ __volatile__(
        "li a0, %[TRAPFRAME]\n"
        // Restore the trap table
        "lw ra,  4 * 0(a0)\n"
        "lw gp,  4 * 1(a0)\n"
        "lw tp,  4 * 2(a0)\n"
        "lw t0,  4 * 3(a0)\n"
        "lw t1,  4 * 4(a0)\n"
        "lw t2,  4 * 5(a0)\n"
        "lw t3,  4 * 6(a0)\n"
        "lw t4,  4 * 7(a0)\n"
        "lw t5,  4 * 8(a0)\n"
        "lw t6,  4 * 9(a0)\n"
        "lw a1,  4 * 11(a0)\n"
        "lw a2,  4 * 12(a0)\n"
        "lw a3,  4 * 13(a0)\n"
        "lw a4,  4 * 14(a0)\n"
        "lw a5,  4 * 15(a0)\n"
        "lw a6,  4 * 16(a0)\n"
        "lw a7,  4 * 17(a0)\n"
        "lw s0,  4 * 18(a0)\n"
        "lw s1,  4 * 19(a0)\n"
        "lw s2,  4 * 20(a0)\n"
        "lw s3,  4 * 21(a0)\n"
        "lw s4,  4 * 22(a0)\n"
        "lw s5,  4 * 23(a0)\n"
        "lw s6,  4 * 24(a0)\n"
        "lw s7,  4 * 25(a0)\n"
        "lw s8,  4 * 26(a0)\n"
        "lw s9,  4 * 27(a0)\n"
        "lw s10, 4 * 28(a0)\n"
        "lw s11, 4 * 29(a0)\n"
        "lw sp,  4 * 30(a0)\n"
        // // We are just overwritng the loaded word
        "lw a0,  4 * 10(a0)\n"
        "sret\n"
        :
        : [TRAPFRAME] "i"(VIRT_TRAP_TABLE));
}

void next_timer()
{
    uint64_t time = READ_CSR(time);
    WRITE_CSR(stimecmp, time + INTERRUPT_TIME);
}

void init_interrupts()
{
    uint32_t sie_val = READ_CSR(sie);
    WRITE_CSR(sie, sie_val | SIE_TIMER_INTERRUPT | SIE_EXTERNAL_INTERRUPT);
    next_timer();
}

void init_trap()
{
    WRITE_CSR(stvec, (uint32_t)trap_entry);
    init_interrupts();
}

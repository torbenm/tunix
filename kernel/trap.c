#include <shrdlib.h>

#include "trap.h"
#include "csr.h"
#include "panic.h"
#include "syscall.h"

void handle_trap(struct trap_frame *f)
{
    uint32_t scause = READ_CSR(scause);
    uint32_t stval = READ_CSR(stval);
    uint32_t user_pc = READ_CSR(sepc);

    if (scause == SCAUSE_ECALL)
    {
        handle_syscall(f);
        user_pc += 4; // increase program counter so that we skip to the next instruction
    }
    else
    {
        PANIC("unexpected trap scause=%x, stval=%x, sepc=%x\n", scause, stval, user_pc);
    }
    WRITE_CSR(sepc, user_pc);
}
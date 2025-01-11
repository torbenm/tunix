#include <shrdlib.h>

#include "trap.h"
#include "panic.h"
#include "process.h"

void handle_syscall(struct trap_frame *f)
{
    switch (f->a3)
    {
    case SYS_PUTCHAR:
        putchar(f->a0);
        break;
    case SYS_GETCHAR:
        while (1)
        {
            long ch = getchar();
            if (ch >= 0)
            {
                f->a0 = ch;
                break;
            }

            yield();
        }
        break;
    case SYS_EXIT:
        exit_current_process();
        break;
    default:
        PANIC("unexpected syscall a3=%x\n", f->a3);
    }
}
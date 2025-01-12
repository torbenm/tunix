#include <shrdlib.h>

#include "trap.h"
#include "panic.h"
#include "proc.h"
#include "tarfs.h"

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
    case SYS_READFILE:
    case SYS_WRITEFILE:
    {
        // TODO: Move this out of here...
        const char *filename = (const char *)f->a0;
        char *buf = (char *)f->a1;
        int len = f->a2;
        struct file *file = fs_lookup(filename);
        if (!file)
        {
            printf("file not found: %s\n", filename);
            f->a0 = -1;
            break;
        }

        if (len > (int)sizeof(file->data))
            len = file->size;

        if (f->a3 == SYS_WRITEFILE)
        {
            memcpy(file->data, buf, len);
            file->size = len;
            fs_flush();
        }
        else
        {
            memcpy(buf, file->data, len);
        }

        f->a0 = len;
        break;
    }
    case SYS_EXIT:
        exit_current_process();
        break;
    case SYS_FORK:
        f->a0 = fork();
        break;
    case SYS_YIELD:
        yield();
        break;
    default:
        PANIC("unexpected syscall a3=%x\n", f->a3);
    }
}
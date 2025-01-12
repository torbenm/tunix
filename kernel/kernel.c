#include <shrdlib.h>

#include "kernel.h"
#include "mem.h"
#include "proc.h"
#include "user_processes.h"
#include "panic.h"
#include "common.h"
#include "trap.h"
#include "virtio.h"
#include "tarfs.h"

extern char __bss[], __bss_end[], __stack_top[];

void init_kernel()
{
    memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

    printf("Tunix started.\n");
}

void kernel_main(void)
{

    init_kernel();
    init_trap();
    init_proc();

    init_virtio_blk();
    init_tarfs();

    start_shell_app();

    // Might be better moved to scheduler
    yield();
    while (1)
    {
        if (!idle())
        {
            printf("All running process have been exited. Shutting down system...");
            shutdown();
        }
    }
}

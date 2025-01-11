#include <shrdlib.h>

#include "kernel.h"
#include "mem.h"
#include "process.h"
#include "user_processes.h"
#include "panic.h"
#include "common.h"
#include "trap.h"

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
    init_process();

    start_shell_app();

    yield();
    printf("No more running processes. Shutting down system...");
    shutdown();
}

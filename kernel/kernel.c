#include <shrdlib.h>

#include "kernel.h"
#include "mem.h"
#include "process.h"
#include "user_processes.h"
#include "panic.h"
#include "common.h"
#include "trap.h"
#include "virtio.h"

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
    init_virtio_blk();

    char buf[SECTOR_SIZE];
    read_write_disk(buf, 0, false /* read from the disk */);
    printf("first sector: %s\n", buf);

    strcpy(buf, "hello from kernel!!!\n");
    read_write_disk(buf, 0, true /* write to the disk */);

    start_shell_app();

    yield();
    printf("No more running processes. Shutting down system...");
    shutdown();
}

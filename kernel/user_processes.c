#include <shrdlib.h>

#include "process.h"

extern char _binary_out_shell_bin_start[], _binary_out_shell_bin_size[];

void start_shell_app()
{
    create_process(_binary_out_shell_bin_start, (size_t)_binary_out_shell_bin_size);
}

void proc_internals(char proc_name)
{
    int x = 0;
    while (1)
    {
        x++;
        printf("%c%d ", proc_name, x);
        yield();

        sleep(30000000);
    }
}

void proc_a_entry(void)
{
    proc_internals('A');
}
void proc_b_entry(void)
{
    proc_internals('B');
}
void proc_c_entry(void)
{
    proc_internals('C');
}
void proc_d_entry(void)
{
    proc_internals('D');
}
void proc_e_entry(void)
{
    int x = 0;
    while (x < 10)
    {
        x++;
        printf("%c%d ", 'E', x);
        yield();

        sleep(30000000);
    }
    return;
}
#include "common.h"
#include "sbi.h"

void sleep(int iterations)
{
    for (int i = 0; i < iterations; i++)
        __asm__ __volatile__("nop");
}

void putchar(char ch)
{
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1);
}

int getchar(void)
{
    struct sbiret ret = sbi_call(0, 0, 0, 0, 0, 0, 0, 2);
    return ret.error;
}

void shutdown(void)
{
    sbi_call(0, 0, 0, 0, 0, 0, 0, 0x8);
}
#include "common.h"
#include "sbi.h"

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
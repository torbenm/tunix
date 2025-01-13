#include <shrdlib.h>
#include <app.h>

void main(void)
{
    printf("Program started.\n");
    int pid = getpid();
    const char x = "abcdefghijklmnopqurstuvwxyz"[pid];
    for (int i = 0; i < 1000; i++)
    {
        putchar(x);
        sleep(3000000);
    }
}
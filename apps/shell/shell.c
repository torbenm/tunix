#include <shrdlib.h>
#include <app.h>

#define MAX_INPUT 128

void background_proc(const char x);

void main(void)
{
    printf("Hello World!!!\n");
    while (1)
    {
        printf("[tunix] ~> ");
        char input[MAX_INPUT];
        readinput(input, MAX_INPUT);

        if (strcmp(input, "hello") == 0)
            printf("Hello world from shell!\n");
        else if (strcmp(input, "exec") == 0)
        {
            printf("Starting bg procs...\n");
            if (fork() == 0)
            {
                background_proc('B');
                return;
            }
            else
            {
                if (fork() == 0)
                {
                    background_proc('A');
                }
                else
                {
                    if (fork() == 0)
                    {
                        background_proc('C');
                    }
                }
            }
        }
        else if (strcmp(input, "q") == 0)
        {
            printf("Shutting system down.");
            shutdown();
        }
        else if (strcmp(input, "exit") == 0)
            return;
        else if (strcmp(input, "readfile") == 0)
        {
            char buf[128];
            int len = readfile("hello.txt", buf, sizeof(buf));
            buf[len] = '\0';
            printf("%s\n", buf);
        }
        else if (strcmp(input, "writefile") == 0)
            writefile("hello.txt", "Hello from shell!\n", 19);
        else
            printf("unknown command: %s\n", input);
    }
}

void background_proc(const char x)
{
    for (int i = 0; i < 1000; i++)
    {
        putchar(x);
        sleep(3000000);
    }
    // printf("Background proc for %c. Will terminate shortly...\n", x);
}
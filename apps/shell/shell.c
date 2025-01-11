#include <shrdlib.h>
#include <app.h>

#define MAX_INPUT 128

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
        else if (strcmp(input, "exit") == 0)
            exit();
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
    for (;;)
        ;
}

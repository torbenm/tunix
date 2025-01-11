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
        else
            printf("unknown command: %s\n", input);
    }
    for (;;)
        ;
}

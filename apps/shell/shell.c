#include <shrdlib.h>
#include <app.h>

#define MAX_INPUT 128
#define MAX_ARG_LEN 32
#define MAX_ARGC 4

#define EVAL_NOMATCH 0
#define EVAL_MATCH 1

#define CMD_CONTINUE 1
#define CMD_EXIT 0

struct Cmd
{
    char cmd[MAX_ARG_LEN];
    int argc;
    char args[MAX_ARGC][MAX_ARG_LEN];
};

void background_proc(const char x);

int eval_cmd(struct Cmd *, char *cmd, int (*callback)(struct Cmd *));
int cmd_exec(struct Cmd *);
int cmd_print(struct Cmd *);
int cmd_shutdown(struct Cmd *);
int cmd_exit(struct Cmd *);
int parse_cmd(struct Cmd *, char *input);

void main(void)
{
    printf("TUNIX Shell initialized.\n");
    while (1)
    {
        printf("\n[tunix] ~> ");
        char input[MAX_INPUT];
        readinput(input, MAX_INPUT);

        struct Cmd cmd;
        if (parse_cmd(&cmd, input) > 0)
        {
            if (eval_cmd(&cmd, "exec", cmd_exec))
                continue;
            if (eval_cmd(&cmd, "print", cmd_print))
                continue;
            if (eval_cmd(&cmd, "q", cmd_shutdown))
                continue;
            if (eval_cmd(&cmd, "exit", cmd_exit))
                continue;
            printf("Unknown command '%s'", cmd.cmd);
        }
    }
}

int eval_cmd(struct Cmd *cmd, char *compare, int (*callback)(struct Cmd *))
{
    if (strcmp(cmd->cmd, compare) == 0)
    {
        if (callback(cmd) == CMD_CONTINUE)
        {
            return EVAL_MATCH;
        }
        else
        {
            printf("Quitting.");
            exit(0);
            return EVAL_MATCH;
        }
    }
    return EVAL_NOMATCH;
}

int parse_cmd(struct Cmd *cmd, char *input)
{
    int i = 0;
    i = strcopyuntil(cmd->cmd, input, i, MAX_ARG_LEN, ' ');
    if (i < 0)
    {
        printf("Error! Argument too long!\n");
        return 0;
    }

    // 0123456789
    // hello world haha
    int a = 0;
    for (i += 1; i > 0 && input[i - 1] != '\0'; a++)
    {
        if (a >= MAX_ARGC)
        {
            printf("Error! Too many args!\n");
            return 0;
        }

        i = strcopyuntil(cmd->args[a], input, i, MAX_ARG_LEN, ' ');
        i++;
    }

    if (i < 0)
    {
        printf("Error! Argument too long!\n");
        return 0;
    }

    cmd->argc = a;
    return 1;
}

int cmd_exec(struct Cmd *cmd)
{
    printf("Starting bg procs...\n");
    if (fork() == 0)
    {
        background_proc('B');
        return CMD_EXIT;
    }
    else
    {
        if (fork() == 0)
        {
            background_proc('A');
            return CMD_EXIT;
        }
        else
        {
            if (fork() == 0)
            {
                background_proc('C');
                return CMD_EXIT;
            }
        }
    }
    return CMD_CONTINUE;
}

int cmd_print(struct Cmd *cmd)
{
    printf("Hello '");
    for (int i = 0; i < cmd->argc; i++)
    {
        printf("%s ", cmd->args[i]);
    }
    printf("'!\n");
    return CMD_CONTINUE;
}

int cmd_shutdown(struct Cmd *cmd)
{
    printf("Shutting system down.");
    shutdown();
}

int cmd_exit(struct Cmd *cmd)
{
    exit(0);
    return CMD_EXIT;
}

void background_proc(const char x)
{
    for (int i = 0; i < 1000; i++)
    {
        putchar(x);
        sleep(3000000);
    }
}
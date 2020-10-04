#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLINE 256
int readline(char *buf)
{
    char c;
    int ret;
    int cnt = 0;
    while ((ret = read(0, &c, 1)) > 0)
    {
        if (c == '\n')
        {
            break;
        }

        cnt++;
        if (cnt > MAXLINE)
        {
            printf("line is too long\n");
            exit(1);
        }
        *buf++ = c;
    }
    if (cnt > 0)
    {
        return cnt;
    }
    return ret;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "args invalid\n");
        exit(1);
    }
    char buf[MAXLINE + 1] = {0};
    int ret = 0;
    memset(buf, 0, MAXLINE + 1);
    while ((ret = readline(buf)) > 0)
    {
        int v = fork();
        if (v == 0)
        {
            // child
            char *args[MAXARG] = {0};

            int i;
            for (i = 1; i < argc; i++)
            {
                args[i - 1] = argv[i];
            }
            args[i - 1] = buf;

            exec(argv[1], args);
            exit(0);
        }
        else if (v > 0)
        {
            // parent
            wait(0);
        }
        else
        {
            fprintf(2, "fork failed\n");
            exit(1);
        }
        memset(buf, 0, MAXLINE + 1);
    }
    exit(0);
}
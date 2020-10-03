#include "kernel/types.h"
#include "user/user.h"

void pw(int fd)
{
    uint8 *b = (uint8 *)malloc(1);
    *b = 0xfe;
    write(fd, b, 1);
    free(b);
}
void pr(int fd)
{
    uint8 *b = (uint8 *)malloc(1);
    read(fd, b, 1);
    if (*b != 0xfe)
    {
        fprintf(2, "%d: received invalid data", getpid());
        exit(1);
    }
    free(b);
}
int main(int argc, char *argv[])
{
    int ping[2];
    if (pipe(ping) != 0)
    {
        fprintf(2, "pipe faild");
        exit(1);
    }

    int pong[2];
    if (pipe(pong) != 0)
    {
        fprintf(2, "pipe faild");
        exit(1);
    }

    int v = fork();
    if (v == 0)
    {
        // child
        pr(ping[0]);
        printf("%d: received ping\n", getpid());
        pw(pong[1]);
    }
    else if (v > 0)
    {
        // parent
        pw(ping[1]);
        pr(pong[0]);
        printf("%d: received pong\n", getpid());
    }
    else
    {
        fprintf(2, "fork failed");
        exit(1);
    }
    exit(0);
}
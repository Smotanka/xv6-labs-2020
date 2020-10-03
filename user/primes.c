#include "kernel/types.h"
#include "user/user.h"

void i2b(int n, uint8 *buf);

int b2i(uint8 buf[4]);
int writeint(int fd, int n);
int readint(int fd, int *out);

void sieve(int in)
{
    int n;
    if (readint(in, &n) != 0)
    {
        return;
    }
    printf("prime %d\n", n);

    int p[2];
    if (pipe(p) != 0)
    {
        fprintf(2, "pipe failed\n");
        exit(1);
    }
    int v = fork();
    if (v == 0)
    {
        close(p[1]);
        sieve(p[0]);
        exit(0);
    }
    else if (v > 0)
    {
        close(p[0]);
        while (1)
        {
            int t;
            if (readint(in, &t) != 0)
            {
                break;
            }
            if (t % n == 0)
            {
                continue;
            }
            writeint(p[1], t);
        }
        close(p[1]);
        wait(0);
    }
    else
    {
        fprintf(2, "fork failed");
        exit(1);
    }
}

void gen(int out)
{
    for (int i = 2; i <= 35; i++)
    {
        writeint(out, i);
    }
    close(out);
}

int b2i(uint8 buf[4])
{
    return buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
}

void i2b(int n, uint8 *buf)
{
    buf[0] = n & 0x000000ff;
    buf[1] = (n & 0x0000ff00) >> 8;
    buf[2] = (n & 0x00ff0000) >> 16;
    buf[3] = (n & 0xff000000) >> 24;
}

int writeint(int fd, int n)
{
    uint8 buf[4];
    i2b(n, buf);
    if (write(fd, buf, 4) == 4)
    {
        return 0;
    }
    return 1;
}

int readint(int fd, int *out)
{
    uint8 buf[4];

    if (read(fd, buf, 4) <= 0)
    {
        return 1;
    }
    *out = b2i(buf);
    return 0;
}

int main(int argc, char *argv[])
{
    int p[2];
    if (pipe(p) != 0)
    {
        fprintf(2, "pipe failed");
        exit(1);
    }

    int v = fork();
    if (v == 0)
    {
        // child
        close(p[1]);
        sieve(p[0]);
        exit(0);
    }
    else if (v > 0)
    {
        // parent
        close(p[0]);
        gen(p[1]);
        wait(0);
    }
    else
    {
        fprintf(2, "fork failed");
        exit(1);
    }
    exit(0);
}
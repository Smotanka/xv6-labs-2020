#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "need more args\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    if (sleep(n) != 0)
    {
        fprintf(2, "sleep failed\n");
        exit(1);
    }
    exit(0);
}
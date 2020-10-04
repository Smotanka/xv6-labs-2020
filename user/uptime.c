#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    uint ticket = uptime();
    printf("ticket: %d\n", ticket);
    exit(0);
}
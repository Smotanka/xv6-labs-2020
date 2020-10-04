#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find_in_dir(char *dir, char *name)
{
    int fd = open(dir, O_RDONLY);
    if (fd < 0)
    {
        fprintf(2, "find failed in dir: %s\n", dir);
        return;
    }
    struct stat st;
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    if (st.type != T_DIR)
    {
        fprintf(2, "%s is not dir\n", dir);
        close(fd);
        return;
    }

    char buf[255] = {0};
    struct dirent de;
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
        {
            break;
        }

        if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
        {
            continue;
        }

        memset(buf, 0, 255);
        int l = strlen(dir);
        memcpy(buf, dir, l);
        *(buf + l) = '/';
        memcpy(buf + l + 1, de.name, strlen(de.name));

        if (strcmp(de.name, name) == 0)
        {
            printf("%s\n", buf);
        }

        struct stat st;
        if (stat(buf, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", dir);
            close(fd);
            return;
        }

        if (st.type == T_DIR)
        {
            find_in_dir(buf, name);
            continue;
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "args invalid\n");
        exit(1);
    }
    find_in_dir(argv[1], argv[2]);
    exit(0);
}

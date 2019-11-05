#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct file_struct {
    int fd;
    char *buf;
    const char *path;
    
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        return 1;

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        return 1;

    char c;
    int size = read(fd, &c, sizeof(c));
    do {
        putchar(c);

        size = read(fd, &c, sizeof(c));
    }while (size > 0);
    return 0;
}

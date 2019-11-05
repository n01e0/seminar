#include <stdio.h>

int main(int argc, const char **argv) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror(argv[1]);
        return 1;
    }

    char buf[16];
    int size = fread(buf, sizeof(char), sizeof(buf), fp);

    do {
        for (int i = 0; i < size; i++)
            putchar(buf[i]);

        size = fread(buf, sizeof(char), sizeof(buf), fp);
    }while (size != 0);
    return 0;
}

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 256

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file(s)>\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp != NULL) {
            char *buf = malloc(BUFSIZE);
            int size = fread(buf, sizeof(char), BUFSIZE, fp);
            do {
                for (int l = 0; l < size; l++)
                    putchar(buf[l]);

                size = fread(buf, sizeof(char), BUFSIZE, fp);
            }while(size > 0);
            free(buf);
        } else {
            fprintf(stderr, "%s %s\n", argv[i], strerror(errno));
        }
    }
    return 0;
}

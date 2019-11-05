#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 1024

typedef struct file_struct {
    int fd;
    int flag;
    char *buf;
    size_t buflen;
    const char *path;
};


file_struct *file_open(const char *path, int flag) {
    file_struct *ret = (*file_struct)malloc(sizeof(file_struct));
    ret->fd = open(path, flag);
    if (ret->fd < 0) {
        file_close(ret);
        return NULL;
    }
    ret->flag = flag;
    ret->buf = (*char)malloc(BUFSIZE);

    ret->buflen = read(ret->fd, ret->buf, sizeof(ret->buf));
    return ret;
}

ssize_t file_buf_fill(file_struct *file) {
    return file->buflen = read(file->fd, file->buf, BUFSIZE);
}

void file_close(file_struct* file) {
    free(file->buf);
    free(file);
}

size_t file_read_bytes(file_struct* file, void* buf, size_t bytes) {
    size_t ret = 0;
    if (file->flag == O_DIRECTORY)
        return -1;

    if (bytes > file->buflen) {
        size_t len = file->buflen;
        memmove(file->buf, buf, file->buflen);
        ret += file->buflen;
        file_buf_fill(file);
        memmove(file->buf, (void*)((char*)buf + (char*)len));
        memmove((file->buf + ), );
    }
}

#include "FILE.h"

size_t __stdio_read(void* restrict, size_t, FILE[restrict static 1]);
size_t __stdio_write(const void* restrict, size_t, FILE[restrict static 1]);
off_t __stdio_seek(FILE[static 1], off_t, int);
int __stdio_close(FILE[static 1]);

#define FILE_(_fd, ...) ((FILE){ \
    .read = __stdio_read,        \
    .write = __stdio_write,      \
    .seek = __stdio_seek,        \
    .close = __stdio_close,      \
    .fd = (_fd),                 \
    __VA_ARGS__                  \
})

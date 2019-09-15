#include "FILE.h"

size_t __stdio_read(void*, size_t, FILE*);
size_t __stdio_write(const void*, size_t, FILE*);
off_t __stdio_seek(FILE*, off_t, int);

#define FILE_(_fd) ((FILE){ \
    .read = __stdio_read,   \
    .write = __stdio_write, \
    .seek = __stdio_seek,   \
    .fd = (_fd)             \
})

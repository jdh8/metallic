#include "FILE.h"
#include <stdint.h>
#include <stdio.h>

typedef int_least64_t off_t;

extern _Thread_local int errno;

off_t __lseek(int, off_t, int);

long __stdio_seek(FILE stream[restrict static 1], long offset, int origin)
{
    off_t position = __lseek(stream->fd, offset, origin);

    if (position < 0) {
        errno = -position;
        return EOF;
    }

    return position;
}


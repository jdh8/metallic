#include "FILE.h"
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

typedef int_least64_t off_t;

off_t __lseek(int, off_t, int);

long __stdio_seek(FILE stream[restrict static 1], long offset, int origin)
{
    off_t position = __lseek(stream->fd, offset, origin);

    if (position > LONG_MAX) {
        errno = EOVERFLOW;
        return EOF;
    }

    if (position < 0) {
        errno = -position;
        return EOF;
    }

    return position;
}


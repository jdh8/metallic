#include "FILE.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>

typedef long long off_t;

long __lseek(int, long, int);
int __llseek(int, long, unsigned long, off_t[static 1], int);

long __stdio_seek(FILE stream[static 1], long offset, int origin)
{
#if LONG_MAX == 0x7FFFFFFF
    off_t position;
    int status = __llseek(stream->fd, offset >> 31, offset, &position, origin);

    if (status < 0) {
        errno = -status;
        return EOF;
    }

    if (position >> 31) {
        errno = EOVERFLOW;
        return EOF;
    }
#else
    off_t position = __lseek(stream->fd, offset, origin);

    if (position < 0) {
        errno = -position;
        return EOF;
    }
#endif

    stream->state &= ~eofbit_;
    return position;
}


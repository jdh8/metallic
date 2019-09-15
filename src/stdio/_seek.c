#include "FILE.h"
#include <errno.h>
#include <limits.h>

long __lseek(int, long, int);
int __llseek(int, long, unsigned long, off_t[static 1], int);

off_t __stdio_seek(FILE stream[static 1], off_t offset, int origin)
{
#if LONG_MAX == 0x7FFFFFFF
    off_t position;
    int status = __llseek(stream->fd, offset >> 31, offset, &position, origin);

    if (status < 0) {
        errno = -status;
        return -1;
    }

    if (position >> 31) {
        errno = EOVERFLOW;
        return -1;
    }
#else
    off_t position = __lseek(stream->fd, offset, origin);

    if (position < 0) {
        errno = -position;
        return -1;
    }
#endif

    stream->state &= ~eofbit_;
    return position;
}


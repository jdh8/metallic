#include "FILE.h"

extern _Thread_local int errno;

long __read(int, void*, size_t);

size_t __stdio_read(void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    long count = __read(stream->fd, buffer, size);

    if (count < 0) {
        stream->state |= _errbit;
        errno = -count;
        return 0;
    }

    stream->state |= (count < size) * _eofbit;
    return count;
}

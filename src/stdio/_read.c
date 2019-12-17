#include "FILE.h"

extern _Thread_local int errno;

long __read(int, void*, size_t);

size_t __stdio_read(FILE stream[restrict static 1], void* restrict buffer, size_t size)
{
    long count = __read(stream->fd, buffer, size);

    if (count >= 0) {
        stream->state |= (count < size) * eofbit_;
        return count;
    }

    stream->state |= errbit_;
    errno = -count;
    return 0;
}

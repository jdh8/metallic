#include "FILE.h"

extern _Thread_local int errno;

long __write(int, const void*, size_t);

size_t __stdio_write(FILE stream[restrict static 1], const void* restrict buffer, size_t size)
{
    long count = __write(stream->fd, buffer, size);

    if (count >= 0) {
        stream->state |= (count < size) * errbit_;
        return count;
    }

    stream->state |= errbit_;
    errno = -count;
    return 0;
}

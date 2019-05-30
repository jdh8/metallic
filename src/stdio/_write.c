#include "FILE.h"

extern _Thread_local int errno;

long __write(int, const void*, size_t);

size_t __stdio_write(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    long result = __write(stream->fd, buffer, size);
    size_t count = result < 0 ? 0 : result;

    if (count < size)
        stream->state |= _errbit;

    if (result < 0)
        errno = -result;

    return count;
}

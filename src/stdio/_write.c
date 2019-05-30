#include "FILE.h"

extern _Thread_local int errno;

ptrdiff_t __write(int, const void*, size_t);

size_t __stdio_write(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    ptrdiff_t status = __write(stream->fd, buffer, size);
    size_t count = status < 0 ? 0 : status;

    if (count < size)
        stream->state |= _errbit;

    if (status < 0)
        errno = -status;

    return count;
}

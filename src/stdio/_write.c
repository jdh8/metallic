#include "FILE.h"
#include <unistd.h>

size_t __stdio_write(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    ssize_t result = write(stream->fd, buffer, size);
    size_t count = result == -1 ? 0 : result;

    if (count < size)
        stream->state |= _errbit;

    return count;
}

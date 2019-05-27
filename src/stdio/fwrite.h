#include "FILE.h"

static size_t _fwrite(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    size_t count = stream->write(buffer, size, stream);

    if (count < size)
        stream->state |= _badbit;

    return count;
}

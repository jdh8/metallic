#include "FILE.h"

static size_t _fread(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    size_t count = stream->read(buffer, size, stream);

    if (count < size)
        stream->state |= _eofbit;

    return count;
}

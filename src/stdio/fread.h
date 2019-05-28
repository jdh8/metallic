#include "FILE.h"

extern _Thread_local int errno;

static size_t _fread(void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    ptrdiff_t count = stream->read(buffer, size, stream);

    if (count < 0) {
        stream->state |= _errbit;
        errno = -count;
        return 0;
    }

    if (count < size)
        stream->state |= _eofbit;

    return count;
}

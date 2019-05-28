#include "FILE.h"

extern _Thread_local int errno;

static size_t _fwrite(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    ptrdiff_t count = stream->write(buffer, size, stream);

    if (count < (ptrdiff_t)size)
        stream->state |= _errbit;

    if (count < 0) {
        errno = -count;
        return 0;
    }

    return count;
}

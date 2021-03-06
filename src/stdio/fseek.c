#include "flush.h"
#include "FILE.h"

int __fseeko(FILE stream[static 1], off_t offset, int origin)
{
    if (origin == SEEK_CUR)
        offset -= stream->avail;

    stream->avail = 0;

    return flush_(stream) || stream->seek(stream, offset, origin) == -1;
}

int fseek(FILE stream[static 1], long offset, int origin)
{
    return __fseeko(stream, offset, origin);
}

#include "FILE.h"
#include <stdio.h>

long ftell(FILE stream[static 1])
{
    ptrdiff_t buffered = stream->base ? stream->ptr - stream->base : 0;
    return stream->seek(stream, 0, SEEK_CUR) - stream->avail + buffered;
}


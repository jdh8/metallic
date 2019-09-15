#include "FILE.h"
#include <stdio.h>

int ungetc(int c, FILE stream[static 1])
{
    if (stream->avail < sizeof(stream->cache) && c != EOF) {
        ++stream->avail;
        stream->cache[sizeof(stream->cache) - stream->avail] = c;
        stream->state &= ~eofbit_;
        return c;
    }

    return EOF;
}


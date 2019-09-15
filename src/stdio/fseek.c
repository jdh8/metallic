#include "flush.h"
#include "FILE.h"
#include <stdio.h>

int fseek(FILE stream[static 1], long offset, int origin)
{
    if (origin == SEEK_CUR)
        offset -= stream->avail;

    return flush_(stream) || stream->seek(stream, offset, origin) == EOF;
}

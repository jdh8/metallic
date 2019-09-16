#include "FILE.h"
#include <stdio.h>

int fsetpos(FILE stream[static 1], const fpos_t position[static 1])
{
    stream->avail = 0;

    return flush_(stream) || stream->seek(stream, position->_offset, SEEK_SET) == -1;
}

#include "FILE.h"
#include <stdio.h>

off_t __ftello(FILE stream[static 1])
{
    ptrdiff_t buffered = stream->base ? stream->ptr - stream->base : 0;
    off_t position = stream->seek(stream, 0, stream->state & appbit_ ? SEEK_END : SEEK_CUR);

    return position == -1 ? -1 : position - stream->avail + buffered;
}

long ftell(FILE stream[static 1])
{
    return __ftello(stream);
}

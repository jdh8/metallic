#include "FILE.h"
#include <stdio.h>

static int flush_(FILE stream[static 1])
{
    stream->avail = 0;

    if (stream->base) {
        stream->write((void*)0, 0, stream);

        if (stream->state & errbit_)
            return EOF;
    }

    return 0;
}

#include "FILE.h"
#include <stdio.h>

static int flush_(FILE stream[static 1])
{
    if (stream->base) {
        stream->write(stream, (void*)0, 0);

        if (stream->state & errbit_)
            return EOF;
    }

    return 0;
}

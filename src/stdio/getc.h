#ifndef METALLIC_GETC_H
#define METALLIC_GETC_H

#include "FILE.h"
#include <stdio.h>

static int getc_(FILE stream[static 1])
{
    if (stream->avail)
        return stream->cache[sizeof(stream->cache) - stream->avail--];

    unsigned char c;
    return stream->read(stream, &c, 1) ? c : EOF;
}

#endif

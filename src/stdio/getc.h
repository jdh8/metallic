#ifndef METALLIC_GETC_H
#define METALLIC_GETC_H

#include "FILE.h"
#include <stdio.h>

extern _Thread_local int errno;

static int _getc(FILE stream[static 1])
{
    unsigned char c;
    int count = stream->read(&c, 1, stream);

    switch (count) {
        case 1:
            return c;
        case 0:
            stream->state |= _eofbit;
            break;
        default:
            stream->state |= _errbit;
            errno = -count;
    }
    return EOF;
}

#endif

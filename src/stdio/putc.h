#ifndef METALLIC_PUTC_H
#define METALLIC_PUTC_H

#include "FILE.h"
#include <stdio.h>

extern _Thread_local int errno;

static int _putc(int c, FILE stream[static 1])
{
    unsigned char character = c;
    int count = stream->write(&character, 1, stream);

    if (count == 1)
        return character;

    stream->state |= _errbit;

    if (count)
        errno = -count;

    return EOF;
}

#endif

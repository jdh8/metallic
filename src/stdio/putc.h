#ifndef METALLIC_PUTC_H
#define METALLIC_PUTC_H

#include "FILE.h"
#include <stdio.h>

static int _putc(int c, FILE stream[static 1])
{
    int result = stream->put(c, stream);

    if (result == EOF)
        stream->state |= _badbit;

    return result;
}

#endif

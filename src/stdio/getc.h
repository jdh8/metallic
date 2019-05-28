#ifndef METALLIC_GETC_H
#define METALLIC_GETC_H

#include "FILE.h"
#include <stdio.h>

static int _getc(FILE stream[static 1])
{
    int result = stream->get(stream);

    if (result == EOF)
        stream->state |= _eofbit;

    return result;
}

#endif

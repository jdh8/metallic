#ifndef METALLIC_GETC_H
#define METALLIC_GETC_H

#include "FILE.h"
#include <stdio.h>

static int getc_(FILE stream[static 1])
{
    unsigned char c;

    return stream->read(&c, 1, stream) ? c : EOF;
}

#endif

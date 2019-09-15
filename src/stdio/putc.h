#ifndef METALLIC_PUTC_H
#define METALLIC_PUTC_H

#include "FILE.h"
#include <stdio.h>

static int putc_(int c, FILE stream[static 1])
{
    unsigned char byte = c;
    
    return stream->write(&byte, 1, stream) == 1 ? byte : EOF;
}

#endif

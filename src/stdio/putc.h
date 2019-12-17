#ifndef METALLIC_PUTC_H
#define METALLIC_PUTC_H

#include "FILE.h"
#include <stdio.h>

static int putc_(FILE stream[static 1], int c)
{
    unsigned char byte = c;
    
    return stream->write(stream, &byte, 1) == 1 ? byte : EOF;
}

#endif

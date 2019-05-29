#ifndef METALLIC_PUTC_H
#define METALLIC_PUTC_H

#include "FILE.h"
#include <stdio.h>

static int _putc(int c, FILE stream[static 1])
{
    unsigned char byte = c;
    
    return stream->write(&byte, 1, stream) ? byte : EOF;
}

#endif

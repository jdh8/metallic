#ifndef METALLIC_GETC_H
#define METALLIC_GETC_H

#include "FILE.h"

static int _getc(FILE stream[static 1])
{
    return stream->get(stream);
}

#endif

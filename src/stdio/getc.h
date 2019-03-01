#ifndef METALLIC_GETC_H
#define METALLIC_GETC_H

#include "FILE.h"

inline int getc(FILE stream[static 1])
{
    return stream->_get(stream);
}

#endif

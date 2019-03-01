#ifndef METALLIC_UNGETC_H
#define METALLIC_UNGETC_H

#include "FILE.h"

inline int ungetc(int c, FILE stream[static 1])
{
    return stream->_unget(c, stream);
}

#endif

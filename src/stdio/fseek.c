#include "FILE.h"
#include <stdio.h>

int fseek(FILE stream[static 1], long offset, int origin)
{
    return stream->seek(stream, offset, origin) == EOF;
}



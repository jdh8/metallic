#include "FILE.h"
#include <stdio.h>

long ftell(FILE stream[static 1])
{
    return stream->seek(stream, 0, SEEK_CUR);
}


#include "FILE.h"
#include <stdio.h>

void rewind(FILE stream[static 1])
{
    fseek(stream, 0, SEEK_SET);
    stream->state &= ~errbit_;
}

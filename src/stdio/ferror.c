#include "FILE.h"

int ferror(FILE stream[static 1])
{
    return stream->state & _errbit;
}

#include "FILE.h"

void clearerr(FILE stream[static 1])
{
    stream->state = 0;
}

#include "FILE.h"

int feof(FILE stream[static 1])
{
    return stream->state & eofbit_;
}

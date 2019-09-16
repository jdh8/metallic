#include "FILE.h"

int __close(int);

int __stdio_close(FILE stream[restrict static 1])
{
    return __close(stream->fd);
}

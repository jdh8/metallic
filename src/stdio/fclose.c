#include "FILE.h"
#include <stdio.h>

extern _Thread_local int errno;

int __close(int);

int fclose(FILE stream[static 1])
{
    int status = __close(stream->fd);

    if (status >= 0)
        return status;

    errno = -status;
    return EOF;
}

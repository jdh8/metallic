#include "flush.h"
#include "FILE.h"
#include <stdio.h>
#include <stdlib.h>

extern _Thread_local int errno;

int __close(int);

int fclose(FILE stream[static 1])
{
    flush_(stream);

    int status = __close(stream->fd);
    free(stream);

    if (status >= 0)
        return status;

    errno = -status;
    return EOF;
}

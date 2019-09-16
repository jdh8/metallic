#include "flush.h"
#include "FILE.h"
#include <stdio.h>
#include <stdlib.h>

extern _Thread_local int errno;

int fclose(FILE stream[static 1])
{
    flush_(stream);

    int status = stream->close(stream);
    free(stream);

    if (status >= 0)
        return 0;

    errno = -status;
    return EOF;
}

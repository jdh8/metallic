#include <stdio.h>
#include <errno.h>

/* WASI preview1 has no canonical /tmp directory; without one we have no
 * portable place to anchor a temporary file. */
FILE* tmpfile(void)
{
    errno = ENOSYS;
    return (void*)0;
}

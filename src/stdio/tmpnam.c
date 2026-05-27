#include <stdio.h>
#include <errno.h>

char* tmpnam(char* s)
{
    (void)s;
    errno = ENOSYS;
    return (void*)0;
}

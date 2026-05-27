#include <stdio.h>
#include <string.h>
#include <errno.h>

void perror(const char* msg)
{
    if (msg && *msg)
        fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    else
        fprintf(stderr, "%s\n", strerror(errno));
}

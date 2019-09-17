#include "modeflags.h"
#include "FILE_.h"
#include <stdlib.h>

extern _Thread_local int errno;

int __open(const char[static 1], int, int);

FILE* fopen(const char path[restrict static 1], const char mode[restrict static 1])
{
    int flags = modeflags_(mode);
    int fd = __open(path, flags, 0666);

    if (fd >= 0) {
        FILE* file = malloc(sizeof(FILE));
        *file = FILE_(fd, .state = flags & O_APPEND ? appbit_ : 0);
        return file;
    }

    errno = -fd;
    return (void*)0;
}

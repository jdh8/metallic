#include "flush.h"
#include "modeflags.h"
#include "FILE_.h"
#include <fcntl.h>
#include <stdlib.h>

extern _Thread_local int errno;

int __open(const char[static 1], int, int);
int __fcntl(int, int, unsigned long);

FILE* freopen(const char path[restrict static 1], const char mode[restrict static 1], FILE stream[restrict static 1])
{
    int flags = modeflags_(mode);
    flush_(stream);

    if (path) {
        stream->close(stream);
        int fd = __open(path, flags, 0666);

        if (fd >= 0) {
            *stream = FILE_(fd);
            return stream;
        }

        errno = -fd;
    }
    else {
        if (flags & O_CLOEXEC)
            __fcntl(stream->fd, F_SETFD, FD_CLOEXEC);

        int status = __fcntl(stream->fd, F_SETFL, flags & ~(O_CREAT | O_EXCL | O_CLOEXEC));

        if (status >= 0)
            return stream;

        errno = -status;
        stream->close(stream);
    }

    free(stream);
    return (void*)0;
}

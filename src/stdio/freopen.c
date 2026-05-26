#include "flush.h"
#include "modeflags.h"
#include "FILE_.h"
#include "../wasi/wasi.h"
#include "../wasi/errno.h"
#include <stdlib.h>

int open_path_(const char *path, int flags);

FILE* freopen(const char path[restrict static 1], const char mode[restrict static 1], FILE stream[restrict static 1])
{
    int flags = modeflags_(mode);
    flush_(stream);

    if (path) {
        stream->close(stream);
        int fd = open_path_(path, flags);

        if (fd >= 0) {
            *stream = FILE_(fd, .state = flags & O_APPEND ? appbit_ : 0);
            return stream;
        }
    }
    else {
        /* WASI has no FD_CLOEXEC concept; O_CLOEXEC has no effect here. */
        __wasi_fdflags_t f = 0;
        if (flags & O_APPEND)   f |= __WASI_FDFLAGS_APPEND;
        if (flags & O_NONBLOCK) f |= __WASI_FDFLAGS_NONBLOCK;
        if (flags & O_DSYNC)    f |= __WASI_FDFLAGS_DSYNC;
        if (flags & O_SYNC)     f |= __WASI_FDFLAGS_SYNC;

        __wasi_errno_t e = __wasi_fd_fdstat_set_flags((__wasi_fd_t)stream->fd, f);
        if (!e) {
            *stream = FILE_(stream->fd);
            return stream;
        }
        errno = wasi_to_posix[e];
        stream->close(stream);
    }

    free(stream);
    return (void*)0;
}

#include <errno.h>
#include <fcntl.h>

#include "wasi.h"
#include "errno.h"

int __fcntl(int fd, int cmd, unsigned long arg)
{
    switch (cmd) {
    case F_GETFL: {
        __wasi_fdstat_t st;
        __wasi_errno_t e = __wasi_fd_fdstat_get((__wasi_fd_t)fd, &st);
        if (e)
            return (int)wasi_check(e);

        int flags = 0;

        /* Access mode bits derived from the rights vector. */
        int can_read  = (st.fs_rights_base & __WASI_RIGHTS_FD_READ)  != 0;
        int can_write = (st.fs_rights_base & __WASI_RIGHTS_FD_WRITE) != 0;
        if (can_read && can_write)
            flags |= O_RDWR;
        else if (can_write)
            flags |= O_WRONLY;
        else
            flags |= O_RDONLY;

        if (st.fs_flags & __WASI_FDFLAGS_APPEND)   flags |= O_APPEND;
        if (st.fs_flags & __WASI_FDFLAGS_NONBLOCK) flags |= O_NONBLOCK;
        if (st.fs_flags & __WASI_FDFLAGS_DSYNC)    flags |= O_DSYNC;
        if (st.fs_flags & __WASI_FDFLAGS_SYNC)     flags |= O_SYNC;

        return flags;
    }

    case F_SETFL: {
        __wasi_fdflags_t f = 0;
        if (arg & O_APPEND)   f |= __WASI_FDFLAGS_APPEND;
        if (arg & O_NONBLOCK) f |= __WASI_FDFLAGS_NONBLOCK;
        if (arg & O_DSYNC)    f |= __WASI_FDFLAGS_DSYNC;
        if (arg & O_SYNC)     f |= __WASI_FDFLAGS_SYNC;
        __wasi_errno_t e = __wasi_fd_fdstat_set_flags((__wasi_fd_t)fd, f);
        return e ? (int)wasi_check(e) : 0;
    }

    case F_GETFD:
    case F_SETFD:
        /* WASI has no FD_CLOEXEC concept. */
        (void)arg;
        return 0;

    default:
        return -EINVAL;
    }
}

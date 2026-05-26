#include <errno.h>
#include <fcntl.h>
#include <stddef.h>

#include "../wasi/wasi.h"
#include "../wasi/errno.h"
#include "../wasi/preopen.h"

static const __wasi_rights_t read_rights =
      __WASI_RIGHTS_FD_READ
    | __WASI_RIGHTS_FD_SEEK
    | __WASI_RIGHTS_FD_TELL
    | __WASI_RIGHTS_FD_FILESTAT_GET
    | __WASI_RIGHTS_FD_READDIR
    | __WASI_RIGHTS_FD_FDSTAT_SET_FLAGS;

static const __wasi_rights_t write_rights =
      __WASI_RIGHTS_FD_WRITE
    | __WASI_RIGHTS_FD_SEEK
    | __WASI_RIGHTS_FD_TELL
    | __WASI_RIGHTS_FD_FILESTAT_GET
    | __WASI_RIGHTS_FD_FILESTAT_SET_SIZE
    | __WASI_RIGHTS_FD_SYNC
    | __WASI_RIGHTS_FD_DATASYNC
    | __WASI_RIGHTS_FD_FDSTAT_SET_FLAGS;

int open_path_(const char *path, int flags)
{
    int basefd;
    const char *rel;
    size_t rel_len;
    if (preopen_lookup(path, &basefd, &rel, &rel_len) < 0)
        return -1;

    __wasi_rights_t rights = 0;
    switch (flags & O_ACCMODE) {
    case O_RDONLY: rights = read_rights;                break;
    case O_WRONLY: rights = write_rights;               break;
    case O_RDWR:   rights = read_rights | write_rights; break;
    default:
        errno = EINVAL;
        return -1;
    }

    if (flags & O_CREAT)
        rights |= __WASI_RIGHTS_PATH_CREATE_FILE;

    __wasi_oflags_t oflags = 0;
    if (flags & O_CREAT)     oflags |= __WASI_OFLAGS_CREAT;
    if (flags & O_EXCL)      oflags |= __WASI_OFLAGS_EXCL;
    if (flags & O_TRUNC)     oflags |= __WASI_OFLAGS_TRUNC;
    if (flags & O_DIRECTORY) oflags |= __WASI_OFLAGS_DIRECTORY;

    __wasi_fdflags_t fdflags = 0;
    if (flags & O_APPEND)   fdflags |= __WASI_FDFLAGS_APPEND;
    if (flags & O_NONBLOCK) fdflags |= __WASI_FDFLAGS_NONBLOCK;
    if (flags & O_DSYNC)    fdflags |= __WASI_FDFLAGS_DSYNC;
    if (flags & O_SYNC)     fdflags |= __WASI_FDFLAGS_SYNC;
    /* O_RSYNC shares its value with O_SYNC in metallic; that's fine. */

    __wasi_lookupflags_t lf = (flags & O_NOFOLLOW)
        ? 0
        : __WASI_LOOKUPFLAGS_SYMLINK_FOLLOW;

    __wasi_fd_t new_fd = 0;
    __wasi_errno_t e = __wasi_path_open(
        (__wasi_fd_t)basefd, lf,
        rel, rel_len,
        oflags,
        rights, rights, /* inheriting == base (over-permissive but compatible) */
        fdflags,
        &new_fd);

    return e ? wasi_seterrno(e) : (int)new_fd;
}

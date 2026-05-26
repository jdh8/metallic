#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/stat.h>

#include "wasi.h"
#include "errno.h"
#include "preopen.h"
#include "stat_convert.h"

static __wasi_rights_t read_rights(void)
{
    return __WASI_RIGHTS_FD_READ
         | __WASI_RIGHTS_FD_SEEK
         | __WASI_RIGHTS_FD_TELL
         | __WASI_RIGHTS_FD_FILESTAT_GET
         | __WASI_RIGHTS_FD_READDIR
         | __WASI_RIGHTS_FD_FDSTAT_SET_FLAGS;
}

static __wasi_rights_t write_rights(void)
{
    return __WASI_RIGHTS_FD_WRITE
         | __WASI_RIGHTS_FD_SEEK
         | __WASI_RIGHTS_FD_TELL
         | __WASI_RIGHTS_FD_FILESTAT_GET
         | __WASI_RIGHTS_FD_FILESTAT_SET_SIZE
         | __WASI_RIGHTS_FD_SYNC
         | __WASI_RIGHTS_FD_DATASYNC
         | __WASI_RIGHTS_FD_FDSTAT_SET_FLAGS;
}

int __open(const char path[static 1], int flags, int mode)
{
    (void)mode;

    int basefd;
    const char *rel;
    size_t rel_len;
    int rc = preopen_lookup(path, &basefd, &rel, &rel_len);
    if (rc < 0)
        return rc;

    __wasi_rights_t rights = 0;
    int access = flags & O_ACCMODE;
    if (access == O_RDONLY)
        rights = read_rights();
    else if (access == O_WRONLY)
        rights = write_rights();
    else if (access == O_RDWR)
        rights = read_rights() | write_rights();
    else
        return -EINVAL;

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

    if (e)
        return (int)wasi_check(e);
    return (int)new_fd;
}

int __unlink(const char path[static 1])
{
    int basefd;
    const char *rel;
    size_t rel_len;
    int rc = preopen_lookup(path, &basefd, &rel, &rel_len);
    if (rc < 0)
        return rc;

    __wasi_errno_t e = __wasi_path_unlink_file((__wasi_fd_t)basefd, rel, rel_len);
    return e ? (int)wasi_check(e) : 0;
}

int __rmdir(const char path[static 1])
{
    int basefd;
    const char *rel;
    size_t rel_len;
    int rc = preopen_lookup(path, &basefd, &rel, &rel_len);
    if (rc < 0)
        return rc;

    __wasi_errno_t e = __wasi_path_remove_directory((__wasi_fd_t)basefd, rel, rel_len);
    return e ? (int)wasi_check(e) : 0;
}

int __rename(const char from[static 1], const char to[static 1])
{
    int from_fd, to_fd;
    const char *from_rel, *to_rel;
    size_t from_len, to_len;

    int rc = preopen_lookup(from, &from_fd, &from_rel, &from_len);
    if (rc < 0)
        return rc;
    rc = preopen_lookup(to, &to_fd, &to_rel, &to_len);
    if (rc < 0)
        return rc;

    __wasi_errno_t e = __wasi_path_rename(
        (__wasi_fd_t)from_fd, from_rel, from_len,
        (__wasi_fd_t)to_fd, to_rel, to_len);
    return e ? (int)wasi_check(e) : 0;
}

static int do_path_stat(const char *path, struct stat *result, __wasi_lookupflags_t lf)
{
    int basefd;
    const char *rel;
    size_t rel_len;
    int rc = preopen_lookup(path, &basefd, &rel, &rel_len);
    if (rc < 0)
        return rc;

    __wasi_filestat_t fs;
    __wasi_errno_t e = __wasi_path_filestat_get((__wasi_fd_t)basefd, lf, rel, rel_len, &fs);
    if (e)
        return (int)wasi_check(e);

    wasi_fill_stat(result, &fs);
    return 0;
}

int __stat(const char path[restrict static 1], struct stat result[restrict static 1])
{
    return do_path_stat(path, result, __WASI_LOOKUPFLAGS_SYMLINK_FOLLOW);
}

int __lstat(const char path[restrict static 1], struct stat result[restrict static 1])
{
    return do_path_stat(path, result, 0);
}

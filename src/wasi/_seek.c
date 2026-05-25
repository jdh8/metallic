#include <stdio.h>
#include "wasi.h"
#include "errno.h"

/* metallic defines off_t as long long in src/stdio/FILE.h. */
typedef long long off_t;

_Static_assert(SEEK_SET == 0, "SEEK_SET must equal __WASI_WHENCE_SET");
_Static_assert(SEEK_CUR == 1, "SEEK_CUR must equal __WASI_WHENCE_CUR");
_Static_assert(SEEK_END == 2, "SEEK_END must equal __WASI_WHENCE_END");

int __llseek(int fd, long offset_hi, unsigned long offset_lo, off_t result[static 1], int whence)
{
    __wasi_filedelta_t offset = ((__wasi_filedelta_t)offset_hi << 32) | (uint32_t)offset_lo;
    __wasi_filesize_t newoff = 0;
    __wasi_errno_t e = __wasi_fd_seek((__wasi_fd_t)fd, offset, (__wasi_whence_t)whence, &newoff);

    if (e)
        return -wasi_to_posix[e];

    *result = (off_t)newoff;
    return 0;
}

long __lseek(int fd, long offset, int whence)
{
    __wasi_filesize_t newoff = 0;
    __wasi_errno_t e = __wasi_fd_seek((__wasi_fd_t)fd, (__wasi_filedelta_t)offset, (__wasi_whence_t)whence, &newoff);

    if (e)
        return -wasi_to_posix[e];

    return (long)newoff;
}

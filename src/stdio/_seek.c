#include "FILE.h"
#include "../wasi/wasi.h"
#include "../wasi/errno.h"
#include <limits.h>

off_t __stdio_seek(FILE stream[static 1], off_t offset, int origin)
{
    __wasi_filesize_t newoff = 0;
    __wasi_errno_t e = __wasi_fd_seek((__wasi_fd_t)stream->fd,
        (__wasi_filedelta_t)offset, (__wasi_whence_t)origin, &newoff);

    if (e)
        return wasi_seterrno(e);

#if LONG_MAX == 0x7FFFFFFF
    if (newoff > (__wasi_filesize_t)0x7FFFFFFF) {
        errno = EOVERFLOW;
        return -1;
    }
#endif

    stream->state &= ~eofbit_;
    return (off_t)newoff;
}

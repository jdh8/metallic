#include "FILE.h"
#include "../wasi/wasi.h"
#include "../wasi/errno.h"

size_t __stdio_write(FILE stream[restrict static 1], const void* restrict buffer, size_t size)
{
    __wasi_ciovec_t iov = { buffer, size };
    size_t nwritten = 0;
    __wasi_errno_t e = __wasi_fd_write((__wasi_fd_t)stream->fd, &iov, 1, &nwritten);

    if (e) {
        stream->state |= errbit_;
        errno = wasi_to_posix[e];
        return 0;
    }

    stream->state |= (nwritten < size) * errbit_;
    return nwritten;
}

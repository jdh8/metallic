#include "FILE.h"
#include "../wasi/wasi.h"
#include "../wasi/errno.h"

size_t __stdio_read(FILE stream[restrict static 1], void* restrict buffer, size_t size)
{
    __wasi_iovec_t iov = { buffer, size };
    size_t nread = 0;
    __wasi_errno_t e = __wasi_fd_read((__wasi_fd_t)stream->fd, &iov, 1, &nread);

    if (e) {
        stream->state |= errbit_;
        errno = wasi_to_posix[e];
        return 0;
    }

    stream->state |= (nread < size) * eofbit_;
    return nread;
}

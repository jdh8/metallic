#include <stddef.h>
#include "wasi.h"
#include "errno.h"

long __read(int fd, void* buf, size_t n)
{
    __wasi_iovec_t iov = { buf, n };
    size_t nread = 0;
    __wasi_errno_t e = __wasi_fd_read((__wasi_fd_t)fd, &iov, 1, &nread);

    return e ? wasi_check(e) : (long)nread;
}

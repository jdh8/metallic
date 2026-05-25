#include <stddef.h>
#include "wasi.h"
#include "errno.h"

long __write(int fd, const void* buf, size_t n)
{
    __wasi_ciovec_t iov = { buf, n };
    size_t nwritten = 0;
    __wasi_errno_t e = __wasi_fd_write((__wasi_fd_t)fd, &iov, 1, &nwritten);

    return e ? wasi_check(e) : (long)nwritten;
}

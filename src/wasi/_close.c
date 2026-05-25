#include "wasi.h"
#include "errno.h"

int __close(int fd)
{
    __wasi_errno_t e = __wasi_fd_close((__wasi_fd_t)fd);
    return e ? -wasi_to_posix[e] : 0;
}

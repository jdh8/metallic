#include "FILE.h"
#include "../wasi/wasi.h"
#include "../wasi/errno.h"

int __stdio_close(FILE stream[restrict static 1])
{
    __wasi_errno_t e = __wasi_fd_close((__wasi_fd_t)stream->fd);
    return e ? wasi_seterrno(e) : 0;
}

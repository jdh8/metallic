#include "../wasi/wasi.h"
#include "../wasi/errno.h"
#include "../wasi/preopen.h"

int rename(const char from[static 1], const char to[static 1])
{
    int from_fd, to_fd;
    const char *from_rel, *to_rel;
    size_t from_len, to_len;

    if (preopen_lookup(from, &from_fd, &from_rel, &from_len) < 0)
        return -1;
    if (preopen_lookup(to, &to_fd, &to_rel, &to_len) < 0)
        return -1;

    __wasi_errno_t e = __wasi_path_rename(
        (__wasi_fd_t)from_fd, from_rel, from_len,
        (__wasi_fd_t)to_fd, to_rel, to_len);
    return e ? wasi_seterrno(e) : 0;
}

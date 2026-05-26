#include <errno.h>

#include "../wasi/wasi.h"
#include "../wasi/errno.h"
#include "../wasi/preopen.h"

int remove(const char path[static 1])
{
    int basefd;
    const char *rel;
    size_t rel_len;
    if (preopen_lookup(path, &basefd, &rel, &rel_len) < 0)
        return -1;

    __wasi_errno_t e = __wasi_path_unlink_file((__wasi_fd_t)basefd, rel, rel_len);

    /* If the entry was a directory, retry as remove_directory.  WASI's
     * unlink_file reports EISDIR (= POSIX 21) on directories. */
    if (wasi_to_posix[e] == EISDIR)
        e = __wasi_path_remove_directory((__wasi_fd_t)basefd, rel, rel_len);

    return e ? wasi_seterrno(e) : 0;
}

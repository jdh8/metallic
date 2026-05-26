#include <sys/stat.h>

#include "../../wasi/wasi.h"
#include "../../wasi/errno.h"
#include "../../wasi/stat_convert.h"

int fstat(int fd, struct stat result[static 1])
{
    __wasi_filestat_t fs;
    __wasi_errno_t e = __wasi_fd_filestat_get((__wasi_fd_t)fd, &fs);
    if (e)
        return wasi_seterrno(e);

    wasi_fill_stat(result, &fs);
    return 0;
}

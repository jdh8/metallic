#include <sys/stat.h>

#include "wasi.h"
#include "errno.h"
#include "stat_convert.h"

int __fstat(int fd, struct stat result[static 1])
{
    __wasi_filestat_t fs;
    __wasi_errno_t e = __wasi_fd_filestat_get((__wasi_fd_t)fd, &fs);
    if (e)
        return (int)wasi_check(e);

    wasi_fill_stat(result, &fs);
    return 0;
}

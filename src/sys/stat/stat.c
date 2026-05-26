#include <sys/stat.h>

#include "../../wasi/wasi.h"
#include "../../wasi/errno.h"
#include "../../wasi/preopen.h"
#include "../../wasi/stat_convert.h"

int stat(const char path[restrict static 1], struct stat result[restrict static 1])
{
    int basefd;
    const char *rel;
    size_t rel_len;
    if (preopen_lookup(path, &basefd, &rel, &rel_len) < 0)
        return -1;

    __wasi_filestat_t fs;
    __wasi_errno_t e = __wasi_path_filestat_get((__wasi_fd_t)basefd,
        __WASI_LOOKUPFLAGS_SYMLINK_FOLLOW, rel, rel_len, &fs);
    if (e)
        return wasi_seterrno(e);

    wasi_fill_stat(result, &fs);
    return 0;
}

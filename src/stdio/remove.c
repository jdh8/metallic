#include "../sys/syscall.h"
#include <errno.h>

int __unlink(const char[static 1]);
int __rmdir(const char[static 1]);

int remove(const char path[static 1])
{
    int result = __unlink(path);

    if (result == -EISDIR)
        result = __rmdir(path);

    return syscall_(result);
}

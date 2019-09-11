#include "../syscall.h"
#include <sys/stat.h>

int __fstat(int, struct stat[static 1]);

int fstat(int fd, struct stat result[static 1])
{
    return syscall_(__fstat(fd, result));
}

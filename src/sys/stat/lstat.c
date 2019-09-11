#include "../syscall.h"
#include <sys/stat.h>

int __lstat(const char[restrict static 1], struct stat[restrict static 1]);

int lstat(const char path[restrict static 1], struct stat result[restrict static 1])
{
    return syscall_(__lstat(path, result));
}

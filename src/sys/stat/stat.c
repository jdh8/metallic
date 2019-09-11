#include "../syscall.h"
#include <sys/stat.h>

int __stat(const char[restrict static 1], struct stat[restrict static 1]);

int stat(const char path[restrict static 1], struct stat result[restrict static 1])
{
    return syscall_(__stat(path, result));
}

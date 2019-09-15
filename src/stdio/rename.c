#include "../sys/syscall.h"

int __rename(const char[static 1], const char[static 1]);

int rename(const char from[static 1], const char to[static 1])
{
    return syscall_(__rename(from, to));
}


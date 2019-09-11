#ifndef METALLIC_SYSCALL_H
#define METALLIC_SYSCALL_H

extern _Thread_local int errno;

static long syscall_(long status)
{
    if (status >= 0)
        return status;

    errno = -status;
    return -1;
}

#endif

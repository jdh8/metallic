#include <time.h>

extern _Thread_local int errno;

int __clock_gettime(unsigned, struct timespec[static 1]);

time_t time(time_t* t)
{
    struct timespec spec;
    int code = __clock_gettime(0, &spec);

    if (code < 0) {
        errno = -code;
        return -1;
    }

    if (t)
        *t = spec.tv_sec;

    return spec.tv_sec;
}

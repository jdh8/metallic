#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>

extern _Thread_local int errno;

int __clock_gettime(clockid_t, struct timespec*);

time_t time(time_t* t)
{
    struct timespec spec;
    int code = __clock_gettime(CLOCK_REALTIME, &spec);

    if (code < 0) {
        errno = -code;
        return -1;
    }

    if (t)
        *t = spec.tv_sec;

    return spec.tv_sec;
}

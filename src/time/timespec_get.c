#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>

#include "../wasi/wasi.h"

int timespec_get(struct timespec* spec, int base)
{
    if (base != TIME_UTC)
        return 0;

    __wasi_timestamp_t now = 0;
    if (__wasi_clock_time_get(__WASI_CLOCKID_REALTIME, 1, &now))
        return 0;

    spec->tv_sec = (time_t)(now / 1000000000u);
    spec->tv_nsec = (long)(now % 1000000000u);
    return base;
}

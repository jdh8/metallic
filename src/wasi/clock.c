#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>
#include "wasi.h"
#include "errno.h"

int __clock_gettime(clockid_t id, struct timespec* ts)
{
    __wasi_timestamp_t now = 0;
    __wasi_errno_t e = __wasi_clock_time_get((__wasi_clockid_t)id, 1, &now);

    if (e)
        return -wasi_to_posix[e];

    ts->tv_sec = (time_t)(now / 1000000000u);
    ts->tv_nsec = (long)(now % 1000000000u);
    return 0;
}

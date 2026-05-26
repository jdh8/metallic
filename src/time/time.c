#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>

#include "../wasi/wasi.h"
#include "../wasi/errno.h"

time_t time(time_t* t)
{
    __wasi_timestamp_t now = 0;
    __wasi_errno_t e = __wasi_clock_time_get(__WASI_CLOCKID_REALTIME, 1, &now);
    if (e) {
        errno = wasi_to_posix[e];
        return (time_t)-1;
    }

    time_t secs = (time_t)(now / 1000000000u);
    if (t)
        *t = secs;
    return secs;
}

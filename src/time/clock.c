#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>

#include "../wasi/wasi.h"

/* C11 §7.27.2.1: clock() returns the implementation's best approximation
 * of CPU time used. CLOCKS_PER_SEC is 1,000,000 in metallic, which
 * matches CLOCK_PROCESS_CPUTIME_ID's microsecond-resolution view of
 * the nanosecond timestamp we get from WASI. */
clock_t clock(void)
{
    __wasi_timestamp_t now = 0;
    if (__wasi_clock_time_get(__WASI_CLOCKID_PROCESS_CPUTIME_ID, 1, &now))
        return (clock_t)-1;

    /* CLOCKS_PER_SEC == 10^6, so divide tv_nsec by 10^3 to convert
     * nanoseconds to microseconds. The expression below stays valid if
     * CLOCKS_PER_SEC is later changed to another power of ten. */
    time_t secs = (time_t)(now / 1000000000u);
    long nsec = (long)(now % 1000000000u);
    return (clock_t)secs * CLOCKS_PER_SEC
         + (clock_t)(nsec / (1000000000L / CLOCKS_PER_SEC));
}

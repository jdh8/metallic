#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>

int __clock_gettime(clockid_t, struct timespec*);

/* C11 §7.27.2.1: clock() returns the implementation's best approximation
 * of CPU time used. CLOCKS_PER_SEC is 1,000,000 in metallic, which
 * matches CLOCK_PROCESS_CPUTIME_ID's microsecond-resolution view of
 * the nanosecond timespec we get from WASI. */
clock_t clock(void)
{
    struct timespec ts;
    if (__clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) < 0)
        return (clock_t)-1;

    /* CLOCKS_PER_SEC == 10^6, so divide tv_nsec by 10^3 to convert
     * nanoseconds to microseconds. The expression below stays valid if
     * CLOCKS_PER_SEC is later changed to another power of ten. */
    return (clock_t)ts.tv_sec * CLOCKS_PER_SEC
         + (clock_t)(ts.tv_nsec / (1000000000L / CLOCKS_PER_SEC));
}

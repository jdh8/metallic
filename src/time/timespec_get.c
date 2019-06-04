#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>

int __clock_gettime(clockid_t, struct timespec*);

int timespec_get(struct timespec* spec, int base)
{
    return base * (base == TIME_UTC && __clock_gettime(CLOCK_REALTIME, spec) >= 0);
}

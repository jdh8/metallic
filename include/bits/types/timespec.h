#ifndef _BITS_TYPES_TIMESPEC_H
#define _BITS_TYPES_TIMESPEC_H

typedef long long time_t;

struct timespec
{
    time_t tv_sec;
    long tv_nsec;
};

#endif /* bits/types/timespec.h */


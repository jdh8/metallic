#ifndef _TIME_H
#define _TIME_H

#include "bits/null.h"
#include "bits/types/timespec.h"

typedef __SIZE_TYPE__ size_t;
typedef long long time_t;
typedef long long clock_t;

struct tm
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

#define CLOCKS_PER_SEC ((clock_t)1000000)
#define TIME_UTC 1

#ifdef __cplusplus
extern "C" {
#endif

time_t time(time_t*);
clock_t clock(void);
double difftime(time_t, time_t);
int timespec_get(struct timespec*, int);

time_t mktime(struct tm*);
struct tm* gmtime(const time_t*);
struct tm* localtime(const time_t*);
size_t strftime(char* __restrict, size_t, const char* __restrict, const struct tm* __restrict);
char* asctime(const struct tm*);
char* ctime(const time_t*);

#ifdef __cplusplus
}
#endif
#endif /* time.h */

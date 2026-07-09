#include "civil.h"
#include <time.h>

struct tm* gmtime(const time_t* t)
{
    static struct tm result;

    time_t secs_total = *t;

    /* Floor-division of secs_total by 86400. C's integer division
     * truncates toward zero, so adjust the quotient downward when the
     * remainder is negative. */
    long long days = (long long)(secs_total / 86400);
    long long secs = (long long)(secs_total % 86400);
    if (secs < 0) {
        secs += 86400;
        days -= 1;
    }

    int year, month, day;
    civil_from_days(days, &year, &month, &day);

    result.tm_sec  = (int)(secs % 60);
    result.tm_min  = (int)((secs / 60) % 60);
    result.tm_hour = (int)(secs / 3600);
    result.tm_mday = day;
    result.tm_mon  = month - 1;
    result.tm_year = year - 1900;

    /* 1970-01-01 (days==0) is a Thursday (wday 4). Use floor-mod on
     * the signed day count so dates before the epoch stay in [0,6]. */
    int wday = (int)(days % 7);
    wday = (wday + 4) % 7;
    if (wday < 0) wday += 7;
    result.tm_wday = wday;

    result.tm_yday = (int)month_yday[is_leap(year)][month - 1] + (day - 1);
    result.tm_isdst = 0;

    return &result;
}

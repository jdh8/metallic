#include <time.h>

/* Howard Hinnant's civil_from_days: take a count of days since the
 * Unix epoch (1970-01-01) and decompose into proleptic Gregorian
 * (year, month, day). The algorithm shifts the epoch to 0000-03-01
 * so that the leap day always falls at the end of a "year", which
 * sidesteps the calendar's month-length irregularities.
 *
 * Reference: http://howardhinnant.github.io/date_algorithms.html
 */
static void civil_from_days(long long days, int* y, int* m, int* d)
{
    days += 719468; /* epoch shift: day 0 becomes 0000-03-01 */
    long long era = (days >= 0 ? days : days - 146096) / 146097;
    unsigned doe = (unsigned)(days - era * 146097);                /* [0, 146096] */
    unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365; /* [0, 399] */
    int year = (int)(yoe) + (int)(era * 400);
    unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);              /* [0, 365] */
    unsigned mp  = (5*doy + 2)/153;                                 /* [0, 11] */
    unsigned dm  = doy - (153*mp + 2)/5 + 1;                        /* [1, 31] */
    unsigned mo  = mp < 10 ? mp + 3 : mp - 9;                       /* [1, 12] */

    *y = (mo <= 2) ? year + 1 : year;
    *m = (int)mo;
    *d = (int)dm;
}

static int is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static const unsigned short month_yday[2][13] = {
    {0, 31, 59, 90,120,151,181,212,243,273,304,334,365},
    {0, 31, 60, 91,121,152,182,213,244,274,305,335,366},
};

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

#ifndef METALLIC_CIVIL_H
#define METALLIC_CIVIL_H

/* Howard Hinnant's civil-date algorithms: convert between a count of
 * days since the Unix epoch (1970-01-01) and proleptic Gregorian
 * (year, month, day). The epoch is shifted to 0000-03-01 so that the
 * leap day always falls at the end of a "year", which sidesteps the
 * calendar's month-length irregularities.
 *
 * Reference: http://howardhinnant.github.io/date_algorithms.html
 */

static inline void civil_from_days(long long days, int* y, int* m, int* d)
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

static inline long long days_from_civil(int y, unsigned m, unsigned d)
{
    y -= m <= 2;
    int era = (y >= 0 ? y : y - 399) / 400;
    unsigned yoe = (unsigned)(y - era * 400);                       /* [0, 399] */
    unsigned doy = (153 * (m > 2 ? m - 3 : m + 9) + 2)/5 + d - 1;   /* [0, 365] */
    unsigned doe = yoe * 365 + yoe/4 - yoe/100 + doy;               /* [0, 146096] */
    return (long long)era * 146097 + (long long)doe - 719468;
}

static inline int is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static const unsigned short month_yday[2][13] = {
    {0, 31, 59, 90,120,151,181,212,243,273,304,334,365},
    {0, 31, 60, 91,121,152,182,213,244,274,305,335,366},
};

#endif

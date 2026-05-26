#include <time.h>

/* Inverse of civil_from_days. Days since the Unix epoch for the
 * given proleptic Gregorian date.
 *
 * Reference: http://howardhinnant.github.io/date_algorithms.html
 */
static long long days_from_civil(int y, unsigned m, unsigned d)
{
    y -= m <= 2;
    int era = (y >= 0 ? y : y - 399) / 400;
    unsigned yoe = (unsigned)(y - era * 400);                       /* [0, 399] */
    unsigned doy = (153 * (m > 2 ? m - 3 : m + 9) + 2)/5 + d - 1;   /* [0, 365] */
    unsigned doe = yoe * 365 + yoe/4 - yoe/100 + doy;               /* [0, 146096] */
    return (long long)era * 146097 + (long long)doe - 719468;
}

static int is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static const unsigned short month_yday[2][13] = {
    {0, 31, 59, 90,120,151,181,212,243,273,304,334,365},
    {0, 31, 60, 91,121,152,182,213,244,274,305,335,366},
};

time_t mktime(struct tm* tm)
{
    /* Normalise tm_mon into [0,11] by carrying overflow into tm_year.
     * C11 §7.27.2.3 explicitly allows the input fields to be out of
     * range; mktime must fold them back. */
    int year  = tm->tm_year + 1900;
    int month = tm->tm_mon;
    if (month < 0 || month > 11) {
        int years = month / 12;
        month %= 12;
        if (month < 0) {
            month += 12;
            years  -= 1;
        }
        year += years;
    }

    long long days = days_from_civil(year, (unsigned)(month + 1), (unsigned)tm->tm_mday);
    long long secs = (long long)tm->tm_hour * 3600
                   + (long long)tm->tm_min  * 60
                   + (long long)tm->tm_sec;

    time_t t = (time_t)(days * 86400 + secs);

    /* Re-derive the broken-down fields so the caller sees the canonical
     * representation, matching glibc/musl behaviour. */
    long long norm_days = (long long)(t / 86400);
    long long norm_secs = (long long)(t % 86400);
    if (norm_secs < 0) {
        norm_secs += 86400;
        norm_days -= 1;
    }

    int wday = (int)(norm_days % 7);
    wday = (wday + 4) % 7;
    if (wday < 0) wday += 7;

    /* Decompose norm_days back through civil_from_days so all fields
     * (including yday) reflect the normalised date. Re-implement inline
     * here rather than depend on gmtime to keep mktime side-effect free
     * with respect to gmtime's static buffer. */
    long long shifted = norm_days + 719468;
    long long era = (shifted >= 0 ? shifted : shifted - 146096) / 146097;
    unsigned doe = (unsigned)(shifted - era * 146097);
    unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
    int cyear = (int)(yoe) + (int)(era * 400);
    unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);
    unsigned mp  = (5*doy + 2)/153;
    unsigned dm  = doy - (153*mp + 2)/5 + 1;
    unsigned mo  = mp < 10 ? mp + 3 : mp - 9;
    int outyear = (mo <= 2) ? cyear + 1 : cyear;

    tm->tm_sec  = (int)(norm_secs % 60);
    tm->tm_min  = (int)((norm_secs / 60) % 60);
    tm->tm_hour = (int)(norm_secs / 3600);
    tm->tm_mday = (int)dm;
    tm->tm_mon  = (int)mo - 1;
    tm->tm_year = outyear - 1900;
    tm->tm_wday = wday;
    tm->tm_yday = (int)month_yday[is_leap(outyear)][mo - 1] + ((int)dm - 1);
    tm->tm_isdst = 0;

    return t;
}

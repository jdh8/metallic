#include "civil.h"
#include <time.h>

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

    int outyear, mo, dm;
    civil_from_days(norm_days, &outyear, &mo, &dm);

    tm->tm_sec  = (int)(norm_secs % 60);
    tm->tm_min  = (int)((norm_secs / 60) % 60);
    tm->tm_hour = (int)(norm_secs / 3600);
    tm->tm_mday = dm;
    tm->tm_mon  = mo - 1;
    tm->tm_year = outyear - 1900;
    tm->tm_wday = wday;
    tm->tm_yday = (int)month_yday[is_leap(outyear)][mo - 1] + (dm - 1);
    tm->tm_isdst = 0;

    return t;
}

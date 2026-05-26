#include <time.h>
#include <assert.h>

int main(void)
{
    /* Epoch — 1970-01-01 00:00:00 UTC, a Thursday. */
    time_t t = 0;
    struct tm* tm = gmtime(&t);
    assert(tm->tm_year == 70);
    assert(tm->tm_mon  == 0);
    assert(tm->tm_mday == 1);
    assert(tm->tm_hour == 0);
    assert(tm->tm_min  == 0);
    assert(tm->tm_sec  == 0);
    assert(tm->tm_wday == 4);
    assert(tm->tm_yday == 0);
    assert(tm->tm_isdst == 0);

    /* 2000-02-29 00:00:00 UTC — leap day, century-rule corner. */
    t = 951782400;
    tm = gmtime(&t);
    assert(tm->tm_year == 100);
    assert(tm->tm_mon  == 1);
    assert(tm->tm_mday == 29);
    assert(tm->tm_hour == 0);
    assert(tm->tm_min  == 0);
    assert(tm->tm_sec  == 0);
    /* 2000-02-29 was a Tuesday. */
    assert(tm->tm_wday == 2);
    /* Day-of-year is zero-based; 31 (Jan) + 28 (Feb 1..28) = 59. */
    assert(tm->tm_yday == 59);

    /* And the noon-of-leap-day version, 12 hours later. */
    t = 951782400 + 12 * 3600;
    tm = gmtime(&t);
    assert(tm->tm_mday == 29);
    assert(tm->tm_hour == 12);

    /* Negative time_t — 1969-12-31 23:59:59 UTC, the second before
     * the epoch. Wednesday. */
    t = -1;
    tm = gmtime(&t);
    assert(tm->tm_year == 69);
    assert(tm->tm_mon  == 11);
    assert(tm->tm_mday == 31);
    assert(tm->tm_hour == 23);
    assert(tm->tm_min  == 59);
    assert(tm->tm_sec  == 59);
    assert(tm->tm_wday == 3);
    assert(tm->tm_yday == 364);

    return 0;
}

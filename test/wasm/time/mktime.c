#include <time.h>
#include <assert.h>

/* Verify that mktime is the inverse of gmtime for a few well-chosen
 * timestamps. (Note: this relies on WASI's "localtime == gmtime"
 * aliasing — once a real timezone database lands this test will need
 * to switch to a UTC-specific helper.) */
static void roundtrip(time_t t)
{
    struct tm tm = *gmtime(&t);
    /* Clear the derived fields so mktime has to recompute them. */
    tm.tm_wday  = -1;
    tm.tm_yday  = -1;
    tm.tm_isdst = -1;
    time_t back = mktime(&tm);
    assert(back == t);
}

int main(void)
{
    roundtrip(0);
    roundtrip(1);
    roundtrip(86400);
    roundtrip(951782400);  /* 2000-02-29 12:00:00 UTC */
    roundtrip(1577836800); /* 2020-01-01 00:00:00 UTC */
    roundtrip(2147483647); /* end of signed-32-bit epoch */
    roundtrip(-1);
    roundtrip(-86400);

    /* Month carry: tm_mon = 13 should fold into next year's February. */
    struct tm tm = {0};
    tm.tm_year = 120;   /* 2020 */
    tm.tm_mon  = 13;    /* February of 2021 after carry */
    tm.tm_mday = 1;
    mktime(&tm);
    assert(tm.tm_year == 121);
    assert(tm.tm_mon  == 1);
    assert(tm.tm_mday == 1);

    /* Negative month: tm_mon = -1 should fold to December of the
     * previous year. */
    tm = (struct tm){0};
    tm.tm_year = 120;
    tm.tm_mon  = -1;
    tm.tm_mday = 15;
    mktime(&tm);
    assert(tm.tm_year == 119);
    assert(tm.tm_mon  == 11);
    assert(tm.tm_mday == 15);

    return 0;
}

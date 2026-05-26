#include <time.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    time_t t = 0;
    struct tm* tm = gmtime(&t);
    char buf[64];

    assert(strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", tm) > 0);
    assert(strcmp(buf, "1970-01-01 00:00:00") == 0);

    /* %e is space-padded — "Thu Jan  1" with two spaces around the 1. */
    assert(strftime(buf, sizeof buf, "%a %b %e", tm) > 0);
    assert(strcmp(buf, "Thu Jan  1") == 0);

    /* Composite specifiers. */
    assert(strftime(buf, sizeof buf, "%F", tm) > 0);
    assert(strcmp(buf, "1970-01-01") == 0);
    assert(strftime(buf, sizeof buf, "%T", tm) > 0);
    assert(strcmp(buf, "00:00:00") == 0);
    assert(strftime(buf, sizeof buf, "%R", tm) > 0);
    assert(strcmp(buf, "00:00") == 0);
    assert(strftime(buf, sizeof buf, "%c", tm) > 0);
    assert(strcmp(buf, "Thu Jan  1 00:00:00 1970") == 0);

    /* %p, %I — midnight is 12 AM under the C locale. */
    assert(strftime(buf, sizeof buf, "%I %p", tm) > 0);
    assert(strcmp(buf, "12 AM") == 0);

    /* %% literal. */
    assert(strftime(buf, sizeof buf, "100%% sure", tm) > 0);
    assert(strcmp(buf, "100% sure") == 0);

    /* Tight buffer: returns 0 when output (including NUL) doesn't fit. */
    assert(strftime(buf, 5, "%Y", tm) == 4);
    assert(strcmp(buf, "1970") == 0);
    assert(strftime(buf, 4, "%Y", tm) == 0);

    return 0;
}

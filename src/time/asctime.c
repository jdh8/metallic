#include <time.h>

extern size_t strftime(char* __restrict, size_t, const char* __restrict, const struct tm* __restrict);

/* C11 §7.27.3.1: equivalent to
 *   sprintf(buf, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n", ...)
 * The "Wed Jan  2 02:03:04 1980\n" form is 25 bytes plus a NUL.
 * Year fields wider than 4 digits violate the standard's buffer
 * assumption; we still write them but truncation can occur for
 * exotic inputs. */
char* asctime(const struct tm* tm)
{
    static char buf[26];
    strftime(buf, sizeof buf, "%a %b %e %H:%M:%S %Y\n", tm);
    return buf;
}

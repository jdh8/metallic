#include <wchar.h>
#include <time.h>

/* M4 stub: wcsftime depends on strftime, which lands in M5.  Return 0
 * for any non-empty format (per C11, the standard return when the result
 * does not fit in the buffer); zero-length formats yield zero too. */
size_t wcsftime(wchar_t* s, size_t maxsize, const wchar_t* format, struct tm* tm)
{
    (void)s;
    (void)maxsize;
    (void)format;
    (void)tm;
    return 0;
}

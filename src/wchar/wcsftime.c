#include <wchar.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

size_t wcsftime(wchar_t* s, size_t maxsize, const wchar_t* format, struct tm* tm)
{
    if (!maxsize)
        return 0;

    mbstate_t st = {0};
    const wchar_t* p = format;
    size_t fmt_bytes = wcsrtombs((void*)0, &p, 0, &st);
    if (fmt_bytes == (size_t)-1)
        return 0;

    char* fmt = malloc(fmt_bytes + 1);
    if (!fmt)
        return 0;
    st = (mbstate_t){0};
    p = format;
    wcsrtombs(fmt, &p, fmt_bytes + 1, &st);

    /* In the C locale, strftime output is ASCII, so a buffer of maxsize
     * bytes is sufficient to hold the corresponding wide-char count. */
    size_t bcap = maxsize * 4;
    char* bbuf = malloc(bcap);
    if (!bbuf) { free(fmt); return 0; }

    size_t bn = strftime(bbuf, bcap, fmt, tm);
    free(fmt);
    if (!bn) { free(bbuf); return 0; }

    size_t wn = mbstowcs(s, bbuf, maxsize);
    free(bbuf);
    if (wn == (size_t)-1 || wn >= maxsize) {
        if (maxsize) s[0] = L'\0';
        return 0;
    }
    return wn;
}

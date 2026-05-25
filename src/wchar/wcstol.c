#include "parse.h"
#include <wchar.h>
#include <stdlib.h>

long wcstol(const wchar_t* s, wchar_t** end, int base)
{
    /* Maximum length: sign, 0x, up to 64 base-2 digits, NUL.  Pad generously. */
    char buf[128];
    wparse_copy_(s, buf, sizeof(buf));

    char* p;
    long v = strtol(buf, &p, base);

    if (end)
        *end = wparse_endptr_(s, buf, p);

    return v;
}

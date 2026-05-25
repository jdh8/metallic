#include "parse.h"
#include <wchar.h>
#include <stdlib.h>

long long wcstoll(const wchar_t* s, wchar_t** end, int base)
{
    char buf[128];
    wparse_copy_(s, buf, sizeof(buf));

    char* p;
    long long v = strtoll(buf, &p, base);

    if (end)
        *end = wparse_endptr_(s, buf, p);

    return v;
}

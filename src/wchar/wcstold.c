#include "parse.h"
#include <wchar.h>
#include <stdlib.h>

long double wcstold(const wchar_t* s, wchar_t** end)
{
    char buf[256];
    wparse_copy_(s, buf, sizeof(buf));

    char* p;
    long double v = strtold(buf, &p);

    if (end)
        *end = wparse_endptr_(s, buf, p);

    return v;
}

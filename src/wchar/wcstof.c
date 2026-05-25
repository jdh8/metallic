#include "parse.h"
#include <wchar.h>
#include <stdlib.h>

float wcstof(const wchar_t* s, wchar_t** end)
{
    char buf[256];
    wparse_copy_(s, buf, sizeof(buf));

    char* p;
    float v = strtof(buf, &p);

    if (end)
        *end = wparse_endptr_(s, buf, p);

    return v;
}

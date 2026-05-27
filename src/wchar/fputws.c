#include <wchar.h>
#include <stdio.h>

int fputws(const wchar_t* restrict s, FILE* restrict stream)
{
    for (; *s; ++s)
        if (fputwc(*s, stream) == WEOF)
            return -1;
    return 0;
}

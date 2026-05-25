#include <wchar.h>

wchar_t* wmemset(wchar_t* destination, wchar_t c, size_t length)
{
    wchar_t* output = destination;

    while (length--)
        *output++ = c;

    return destination;
}

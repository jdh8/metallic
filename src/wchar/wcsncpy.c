#include <wchar.h>

wchar_t* wcsncpy(wchar_t destination[static restrict 1], const wchar_t source[static restrict 1], size_t length)
{
    wchar_t* output = destination;

    for (; length && *source; --length)
        *output++ = *source++;

    while (length--)
        *output++ = 0;

    return destination;
}

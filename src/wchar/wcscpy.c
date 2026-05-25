#include <wchar.h>

wchar_t* wcscpy(wchar_t destination[static restrict 1], const wchar_t source[static restrict 1])
{
    wchar_t* output = destination;

    while ((*output++ = *source++));

    return destination;
}

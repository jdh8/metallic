#include <wchar.h>

wchar_t* wcscat(wchar_t destination[static restrict 1], const wchar_t source[static restrict 1])
{
    wcscpy(destination + wcslen(destination), source);

    return destination;
}

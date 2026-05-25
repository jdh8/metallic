#include <wchar.h>
#include <string.h>

wchar_t* wmemmove(wchar_t* destination, const wchar_t* source, size_t length)
{
    memmove(destination, source, length * sizeof(wchar_t));
    return destination;
}

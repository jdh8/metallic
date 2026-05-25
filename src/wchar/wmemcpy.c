#include <wchar.h>
#include <string.h>

wchar_t* wmemcpy(wchar_t* restrict destination, const wchar_t* restrict source, size_t length)
{
    memcpy(destination, source, length * sizeof(wchar_t));
    return destination;
}

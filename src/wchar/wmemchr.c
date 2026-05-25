#include <wchar.h>

wchar_t* wmemchr(const wchar_t* source, wchar_t c, size_t length)
{
    for (; length--; ++source)
        if (*source == c)
            return (wchar_t*)source;

    return 0;
}

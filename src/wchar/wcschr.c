#include <wchar.h>

wchar_t* wcschr(const wchar_t source[static 1], wchar_t c)
{
    for (; *source; ++source)
        if (*source == c)
            return (wchar_t*)source;

    return c ? 0 : (wchar_t*)source;
}

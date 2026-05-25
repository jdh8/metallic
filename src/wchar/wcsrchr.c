#include <wchar.h>

wchar_t* wcsrchr(const wchar_t source[static 1], wchar_t c)
{
    const wchar_t* found = 0;

    for (; *source; ++source)
        if (*source == c)
            found = source;

    if (!c)
        return (wchar_t*)source;

    return (wchar_t*)found;
}

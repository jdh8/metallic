#include <wchar.h>

wchar_t* wcstok(wchar_t* restrict string, const wchar_t* restrict separators, wchar_t** restrict state)
{
    if (!(string || (string = *state)))
        return 0;

    string += wcsspn(string, separators);

    if (!*string)
        return *state = 0;

    *state = wcspbrk(string, separators);

    if (*state)
        *(*state)++ = 0;

    return string;
}

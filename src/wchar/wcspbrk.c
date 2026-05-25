#include <wchar.h>

wchar_t* wcspbrk(const wchar_t string[static 1], const wchar_t set[static 1])
{
    for (; *string; ++string)
        if (wcschr(set, *string))
            return (wchar_t*)string;

    return 0;
}

#include <wchar.h>

wchar_t* wcsstr(const wchar_t haystack[static 1], const wchar_t needle[static 1])
{
    if (!*needle)
        return (wchar_t*)haystack;

    for (; *haystack; ++haystack) {
        const wchar_t* h = haystack;
        const wchar_t* n = needle;

        while (*h && *n && *h == *n) {
            ++h;
            ++n;
        }

        if (!*n)
            return (wchar_t*)haystack;
    }

    return 0;
}

#include <wchar.h>

size_t wcslen(const wchar_t begin[static 1])
{
    const wchar_t* end = begin;

    while (*end)
        ++end;

    return end - begin;
}

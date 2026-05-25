#include <stdlib.h>
#include <wchar.h>

size_t wcstombs(char* dst, const wchar_t* src, size_t len)
{
    mbstate_t state = {0};
    const wchar_t* p = src;
    return wcsrtombs(dst, &p, len, &state);
}

#include <stdlib.h>
#include <wchar.h>

size_t mbstowcs(wchar_t* dst, const char* src, size_t len)
{
    mbstate_t state = {0};
    const char* p = src;
    return mbsrtowcs(dst, &p, len, &state);
}

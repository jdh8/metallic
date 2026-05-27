#include <uchar.h>
#include <wchar.h>

size_t c32rtomb(char* restrict s, char32_t c, mbstate_t* restrict ps)
{
    return wcrtomb(s, (wchar_t)c, ps);
}

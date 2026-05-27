#include <uchar.h>
#include <wchar.h>

size_t mbrtoc32(char32_t* restrict pc, const char* restrict s, size_t n, mbstate_t* restrict ps)
{
    wchar_t wc;
    size_t r = mbrtowc(&wc, s, n, ps);

    if (pc && r != (size_t)-1 && r != (size_t)-2)
        *pc = (char32_t)wc;

    return r;
}

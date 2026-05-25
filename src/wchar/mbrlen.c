#include <wchar.h>

size_t mbrlen(const char* restrict s, size_t n, mbstate_t* restrict ps)
{
    static mbstate_t internal;
    return mbrtowc(0, s, n, ps ? ps : &internal);
}

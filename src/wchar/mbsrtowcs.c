#include <wchar.h>

size_t mbsrtowcs(wchar_t* restrict dst, const char** restrict src, size_t len, mbstate_t* restrict ps)
{
    static mbstate_t internal;
    if (!ps)
        ps = &internal;

    const char* p = *src;
    size_t written = 0;

    if (dst) {
        while (written < len) {
            wchar_t wc;
            size_t r = mbrtowc(&wc, p, (size_t)-1 / 2, ps);

            if (r == (size_t)-1) {
                *src = p;
                return (size_t)-1;
            }

            dst[written] = wc;

            if (!wc) {
                *src = 0;
                return written;
            }

            p += r;
            ++written;
        }

        *src = p;
        return written;
    }

    /* No destination: just count, without advancing *src */
    for (;;) {
        wchar_t wc;
        size_t r = mbrtowc(&wc, p, (size_t)-1 / 2, ps);

        if (r == (size_t)-1)
            return (size_t)-1;

        if (!wc)
            return written;

        p += r;
        ++written;
    }
}

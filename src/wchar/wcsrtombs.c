#include <wchar.h>
#include <errno.h>

size_t wcsrtombs(char* restrict dst, const wchar_t** restrict src, size_t len, mbstate_t* restrict ps)
{
    static mbstate_t internal;
    if (!ps)
        ps = &internal;

    const wchar_t* p = *src;
    size_t written = 0;

    if (dst) {
        char buf[4];

        while (written < len) {
            size_t need;

            if (!*p) {
                /* Encode the terminating NUL only if it fits. */
                if (written >= len)
                    break;
                dst[written++] = 0;
                *src = 0;
                return written - 1;
            }

            need = wcrtomb(buf, *p, ps);

            if (need == (size_t)-1) {
                *src = p;
                return (size_t)-1;
            }

            if (written + need > len) {
                *src = p;
                return written;
            }

            for (size_t i = 0; i < need; ++i)
                dst[written + i] = buf[i];

            written += need;
            ++p;
        }

        *src = p;
        return written;
    }

    /* No destination: just count. */
    for (;;) {
        char buf[4];

        if (!*p)
            return written;

        size_t need = wcrtomb(buf, *p, ps);

        if (need == (size_t)-1)
            return (size_t)-1;

        written += need;
        ++p;
    }
}

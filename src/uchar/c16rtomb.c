#include <uchar.h>
#include <wchar.h>
#include <errno.h>
#include <stdint.h>

/* Surrogate state lives in mbstate_t.__code:
 *   In [0xD800, 0xDBFF]: a high surrogate from a previous c16rtomb call, to be
 *                        combined with the low surrogate now arriving. */

size_t c16rtomb(char* restrict s, char16_t c, mbstate_t* restrict ps)
{
    static mbstate_t internal;
    if (!ps)
        ps = &internal;

    uint_least32_t pending = ps->__code;

    if (c - 0xD800u < 0x400u) {
        ps->__code = c;
        return 0;
    }

    if (c - 0xDC00u < 0x400u) {
        if (pending - 0xD800u >= 0x400u) {
            errno = EILSEQ;
            return (size_t)-1;
        }
        uint_least32_t cp = 0x10000 + (((pending & 0x3FF) << 10) | (c & 0x3FF));
        ps->__code = 0;
        return wcrtomb(s, (wchar_t)cp, ps);
    }

    if (pending - 0xD800u < 0x400u) {
        errno = EILSEQ;
        return (size_t)-1;
    }

    return wcrtomb(s, (wchar_t)c, ps);
}

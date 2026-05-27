#include <uchar.h>
#include <wchar.h>
#include <stdint.h>

/* Surrogate state lives in mbstate_t.__code:
 *   In [0xDC00, 0xDFFF]: a low surrogate stashed by a previous mbrtoc16 call,
 *                        to be returned on the next call with (size_t)-3.
 * mbrtowc's mid-sequence encoding (bits 28..31 hold total >= 2) never collides
 * with this range. */

size_t mbrtoc16(char16_t* restrict pc, const char* restrict s, size_t n, mbstate_t* restrict ps)
{
    static mbstate_t internal;
    if (!ps)
        ps = &internal;

    if (ps->__code - 0xDC00u < 0x400u) {
        if (pc)
            *pc = (char16_t)ps->__code;
        ps->__code = 0;
        return (size_t)-3;
    }

    wchar_t wc;
    size_t r = mbrtowc(&wc, s, n, ps);

    if (r == (size_t)-1 || r == (size_t)-2)
        return r;

    uint_least32_t cp = (uint_least32_t)wc;

    if (cp < 0x10000) {
        if (pc)
            *pc = (char16_t)cp;
        return r;
    }

    uint_least32_t v = cp - 0x10000;
    if (pc)
        *pc = (char16_t)(0xD800 | (v >> 10));
    ps->__code = 0xDC00u | (v & 0x3FFu);
    return r;
}

/* Native test for metallic's UTF-8 multibyte conversion routines.
 *
 * We must use metallic's mbstate_t (single unsigned __code slot), not the
 * host libc's opaque struct.  To force that, we include metallic's
 * <wchar.h> via a quoted path *before* anything else.  Its include guard
 * (`_WCHAR_H`) then suppresses any later inclusion of the system header.
 *
 * The metallic implementations are renamed via macros to avoid clashing
 * with the host libc's wide-character entry points at link time. */

#include "include/wchar.h"

#define wcrtomb   m_wcrtomb
#define mbrtowc   m_mbrtowc
#define mbrlen    m_mbrlen
#define mbsinit   m_mbsinit
#define mbsrtowcs m_mbsrtowcs
#define wcsrtombs m_wcsrtombs
#define btowc     m_btowc
#define wctob     m_wctob

#include "src/wchar/wcrtomb.c"
#include "src/wchar/mbrtowc.c"
#include "src/wchar/mbrlen.c"
#include "src/wchar/mbsinit.c"
#include "src/wchar/mbsrtowcs.c"
#include "src/wchar/wcsrtombs.c"
#include "src/wchar/btowc.c"
#include "src/wchar/wctob.c"

#include <assert.h>
#include <errno.h>
#include <wchar.h>
#include <stdio.h>
#include <string.h>

static size_t metallic_strlen(const char* s)
{
    size_t n = 0;
    while (s[n])
        ++n;
    return n;
}

int main(void)
{
    /* mbsinit on null state */
    assert(m_mbsinit(0));
    mbstate_t st = {0};
    assert(m_mbsinit(&st));

    /* Round-trip ASCII */
    {
        mbstate_t s = {0};
        wchar_t wc;
        assert(m_mbrtowc(&wc, "A", 1, &s) == 1);
        assert(wc == L'A');
        assert(m_mbsinit(&s));
    }

    /* "héllo" : h(1) + é(2) + l(1) + l(1) + o(1) = 6 bytes -> 5 wchars */
    {
        const char* src = "h\xC3\xA9llo";
        assert(metallic_strlen(src) == 6);
        mbstate_t s = {0};
        wchar_t out[8];
        size_t n = 0;
        const char* p = src;
        while (*p) {
            size_t r = m_mbrtowc(&out[n], p, 4, &s);
            assert(r != (size_t)-1 && r != (size_t)-2);
            p += r;
            ++n;
        }
        assert(n == 5);
        assert(out[0] == L'h');
        assert(out[1] == 0xE9); /* é */
        assert(out[2] == L'l');
        assert(out[3] == L'l');
        assert(out[4] == L'o');
    }

    /* "日本語" : 3 codepoints, 9 bytes (3 bytes each) */
    {
        const char* src = "\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E";
        assert(metallic_strlen(src) == 9);
        mbstate_t s = {0};
        wchar_t out[4];
        size_t n = 0;
        const char* p = src;
        while (*p) {
            size_t r = m_mbrtowc(&out[n], p, 4, &s);
            assert(r == 3);
            p += r;
            ++n;
        }
        assert(n == 3);
        assert(out[0] == 0x65E5);
        assert(out[1] == 0x672C);
        assert(out[2] == 0x8A9E);
    }

    /* "🦀" : 1 codepoint U+1F980, 4 bytes — exercises the wcrtomb 4-byte fix */
    {
        const char* src = "\xF0\x9F\xA6\x80";
        mbstate_t s = {0};
        wchar_t wc;
        size_t r = m_mbrtowc(&wc, src, 4, &s);
        assert(r == 4);
        assert(wc == 0x1F980);

        /* Round-trip back to UTF-8 via wcrtomb */
        char buf[8] = {0};
        mbstate_t s2 = {0};
        size_t k = m_wcrtomb(buf, wc, &s2);
        assert(k == 4); /* this is the bug-fix line */
        assert((unsigned char)buf[0] == 0xF0);
        assert((unsigned char)buf[1] == 0x9F);
        assert((unsigned char)buf[2] == 0xA6);
        assert((unsigned char)buf[3] == 0x80);
    }

    /* Partial sequence: feed only 2 bytes of a 3-byte sequence, expect -2,
     * then feed the rest, expect completion. */
    {
        const char* src = "\xE6\x97\xA5"; /* 日 */
        mbstate_t s = {0};
        wchar_t wc = 0;
        size_t r1 = m_mbrtowc(&wc, src, 2, &s);
        assert(r1 == (size_t)-2);
        assert(!m_mbsinit(&s));
        size_t r2 = m_mbrtowc(&wc, src + 2, 1, &s);
        assert(r2 == 1);
        assert(wc == 0x65E5);
        assert(m_mbsinit(&s));
    }

    /* Partial sequence: 1-byte slices of a 4-byte sequence */
    {
        const char* src = "\xF0\x9F\xA6\x80"; /* 🦀 */
        mbstate_t s = {0};
        wchar_t wc = 0;
        assert(m_mbrtowc(&wc, src + 0, 1, &s) == (size_t)-2);
        assert(m_mbrtowc(&wc, src + 1, 1, &s) == (size_t)-2);
        assert(m_mbrtowc(&wc, src + 2, 1, &s) == (size_t)-2);
        assert(m_mbrtowc(&wc, src + 3, 1, &s) == 1);
        assert(wc == 0x1F980);
    }

    /* Invalid sequence: lone continuation byte */
    {
        mbstate_t s = {0};
        errno = 0;
        size_t r = m_mbrtowc(0, "\x80", 1, &s);
        assert(r == (size_t)-1);
        assert(errno == EILSEQ);
    }

    /* Invalid sequence: 5-byte lead (>= 0xF8) */
    {
        mbstate_t s = {0};
        errno = 0;
        assert(m_mbrtowc(0, "\xF8", 1, &s) == (size_t)-1);
        assert(errno == EILSEQ);
    }

    /* Overlong: 2-byte form of an ASCII char (0xC0 0x80) */
    {
        mbstate_t s = {0};
        errno = 0;
        assert(m_mbrtowc(0, "\xC0\x80", 2, &s) == (size_t)-1);
        assert(errno == EILSEQ);
    }

    /* Surrogate U+D800 encoded as 0xED 0xA0 0x80 must be rejected */
    {
        mbstate_t s = {0};
        errno = 0;
        assert(m_mbrtowc(0, "\xED\xA0\x80", 3, &s) == (size_t)-1);
        assert(errno == EILSEQ);
    }

    /* Bad continuation: lead followed by non-continuation byte */
    {
        mbstate_t s = {0};
        errno = 0;
        assert(m_mbrtowc(0, "\xC3" "A", 2, &s) == (size_t)-1);
        assert(errno == EILSEQ);
    }

    /* mbrtowc with s == NULL resets state */
    {
        mbstate_t s;
        s.__code = 0x12345; /* arbitrary "in-progress" */
        assert(m_mbrtowc(0, 0, 0, &s) == 0);
        assert(m_mbsinit(&s));
    }

    /* mbrtowc returning 0 for L'\0' */
    {
        mbstate_t s = {0};
        wchar_t wc = 0xDEAD;
        size_t r = m_mbrtowc(&wc, "", 1, &s);
        assert(r == 0);
        assert(wc == 0);
    }

    /* mbrlen mirrors mbrtowc */
    {
        mbstate_t s = {0};
        assert(m_mbrlen("\xE6\x97\xA5", 3, &s) == 3);
        assert(m_mbrlen("\xE6\x97", 2, &s) == (size_t)-2);
    }

    /* btowc / wctob */
    assert(m_btowc(EOF) == WEOF);
    assert(m_btowc('A') == L'A');
    assert(m_btowc(0x80) == WEOF);
    assert(m_btowc(0xFF) == WEOF);
    assert(m_wctob(L'A') == 'A');
    assert(m_wctob(0) == 0);
    assert(m_wctob(0x80) == EOF);
    assert(m_wctob(0x10FFFF) == EOF);

    /* wcrtomb encodes 1/2/3/4 byte sequences */
    {
        char buf[8];
        mbstate_t s = {0};
        assert(m_wcrtomb(buf, L'A', &s) == 1);
        assert(buf[0] == 'A');
        assert(m_wcrtomb(buf, 0xE9, &s) == 2); /* é */
        assert((unsigned char)buf[0] == 0xC3 && (unsigned char)buf[1] == 0xA9);
        assert(m_wcrtomb(buf, 0x65E5, &s) == 3); /* 日 */
        assert((unsigned char)buf[0] == 0xE6);
        assert((unsigned char)buf[1] == 0x97);
        assert((unsigned char)buf[2] == 0xA5);
        assert(m_wcrtomb(buf, 0x1F980, &s) == 4); /* 🦀 — bug-fix */
        assert((unsigned char)buf[0] == 0xF0);
        assert((unsigned char)buf[1] == 0x9F);
        assert((unsigned char)buf[2] == 0xA6);
        assert((unsigned char)buf[3] == 0x80);
    }

    /* mbsrtowcs full string */
    {
        const char* src = "h\xC3\xA9llo\xE6\x97\xA5\xF0\x9F\xA6\x80";
        const char* p = src;
        wchar_t buf[16] = {0};
        mbstate_t s = {0};
        size_t n = m_mbsrtowcs(buf, &p, 16, &s);
        assert(n == 7);
        assert(p == 0); /* terminator consumed */
        assert(buf[0] == L'h');
        assert(buf[1] == 0xE9);
        assert(buf[2] == L'l');
        assert(buf[3] == L'l');
        assert(buf[4] == L'o');
        assert(buf[5] == 0x65E5);
        assert(buf[6] == 0x1F980);
        assert(buf[7] == 0);
    }

    /* mbsrtowcs counting (dst == NULL) */
    {
        const char* src = "h\xC3\xA9llo";
        const char* p = src;
        mbstate_t s = {0};
        size_t n = m_mbsrtowcs(0, &p, 0, &s);
        assert(n == 5);
        assert(p == src); /* unchanged when dst == NULL */
    }

    /* wcsrtombs full string */
    {
        const wchar_t src[] = {L'h', 0xE9, L'l', L'l', L'o', 0x1F980, 0};
        const wchar_t* p = src;
        char buf[16] = {0};
        mbstate_t s = {0};
        size_t n = m_wcsrtombs(buf, &p, 16, &s);
        assert(n == 1 + 2 + 1 + 1 + 1 + 4);
        assert(p == 0);
        assert((unsigned char)buf[0] == 'h');
        assert((unsigned char)buf[1] == 0xC3);
        assert((unsigned char)buf[2] == 0xA9);
        assert((unsigned char)buf[5] == 'o');
        assert((unsigned char)buf[6] == 0xF0);
        assert((unsigned char)buf[9] == 0x80);
        assert(buf[10] == 0);
    }

    /* wcsrtombs counting */
    {
        const wchar_t src[] = {L'h', 0xE9, 0x1F980, 0};
        const wchar_t* p = src;
        mbstate_t s = {0};
        size_t n = m_wcsrtombs(0, &p, 0, &s);
        assert(n == 1 + 2 + 4);
    }

    return 0;
}

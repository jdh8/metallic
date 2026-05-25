#include <assert.h>
#include <errno.h>
#include <stdio.h> /* EOF */
#include <wchar.h>

int main(void)
{
    /* mbsinit on null state */
    assert(mbsinit(0));
    mbstate_t st = {0};
    assert(mbsinit(&st));

    /* "héllo" -> 6 bytes / 5 wchars */
    {
        const char* src = "h\xC3\xA9llo";
        mbstate_t s = {0};
        wchar_t out[8];
        size_t n = 0;
        const char* p = src;
        while (*p) {
            size_t r = mbrtowc(&out[n], p, 4, &s);
            assert(r != (size_t)-1 && r != (size_t)-2);
            p += r;
            ++n;
        }
        assert(n == 5);
        assert(out[0] == L'h');
        assert(out[1] == 0xE9);
        assert(out[4] == L'o');
    }

    /* "日本語" -> 9 bytes / 3 wchars */
    {
        const char* src = "\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E";
        mbstate_t s = {0};
        wchar_t out[4];
        const char* p = src;
        size_t n = 0;
        while (*p) {
            size_t r = mbrtowc(&out[n], p, 4, &s);
            assert(r == 3);
            p += r;
            ++n;
        }
        assert(n == 3);
        assert(out[0] == 0x65E5);
        assert(out[1] == 0x672C);
        assert(out[2] == 0x8A9E);
    }

    /* "🦀" -> 4 bytes / 1 wchar (exercises wcrtomb's 4-byte fix) */
    {
        const char* src = "\xF0\x9F\xA6\x80";
        mbstate_t s = {0};
        wchar_t wc;
        size_t r = mbrtowc(&wc, src, 4, &s);
        assert(r == 4);
        assert(wc == 0x1F980);

        char buf[8];
        mbstate_t s2 = {0};
        size_t k = wcrtomb(buf, wc, &s2);
        assert(k == 4);
        assert((unsigned char)buf[0] == 0xF0);
        assert((unsigned char)buf[3] == 0x80);
    }

    /* Partial sequence */
    {
        const char* src = "\xE6\x97\xA5";
        mbstate_t s = {0};
        wchar_t wc = 0;
        assert(mbrtowc(&wc, src, 2, &s) == (size_t)-2);
        assert(!mbsinit(&s));
        assert(mbrtowc(&wc, src + 2, 1, &s) == 1);
        assert(wc == 0x65E5);
        assert(mbsinit(&s));
    }

    /* Invalid: lone continuation byte */
    {
        mbstate_t s = {0};
        errno = 0;
        assert(mbrtowc(0, "\x80", 1, &s) == (size_t)-1);
        assert(errno == EILSEQ);
    }

    /* Overlong */
    {
        mbstate_t s = {0};
        errno = 0;
        assert(mbrtowc(0, "\xC0\x80", 2, &s) == (size_t)-1);
        assert(errno == EILSEQ);
    }

    /* Surrogate */
    {
        mbstate_t s = {0};
        errno = 0;
        assert(mbrtowc(0, "\xED\xA0\x80", 3, &s) == (size_t)-1);
        assert(errno == EILSEQ);
    }

    /* mbrtowc(NULL, NULL, 0, &s) resets */
    {
        mbstate_t s;
        s.__code = 0x12345;
        assert(mbrtowc(0, 0, 0, &s) == 0);
        assert(mbsinit(&s));
    }

    /* mbrlen */
    {
        mbstate_t s = {0};
        assert(mbrlen("\xE6\x97\xA5", 3, &s) == 3);
        assert(mbrlen("\xE6\x97", 2, &s) == (size_t)-2);
    }

    /* btowc / wctob */
    assert(btowc(EOF) == WEOF);
    assert(btowc('A') == L'A');
    assert(btowc(0x80) == WEOF);
    assert(wctob(L'A') == 'A');
    assert(wctob(0x80) == EOF);

    /* mbsrtowcs */
    {
        const char* src = "h\xC3\xA9llo";
        const char* p = src;
        wchar_t buf[8] = {0};
        mbstate_t s = {0};
        size_t n = mbsrtowcs(buf, &p, 8, &s);
        assert(n == 5);
        assert(p == 0);
        assert(buf[0] == L'h' && buf[1] == 0xE9 && buf[4] == L'o' && buf[5] == 0);
    }

    /* wcsrtombs */
    {
        const wchar_t src[] = {L'h', 0xE9, 0x1F980, 0};
        const wchar_t* p = src;
        char buf[16] = {0};
        mbstate_t s = {0};
        size_t n = wcsrtombs(buf, &p, 16, &s);
        assert(n == 1 + 2 + 4);
        assert(p == 0);
        assert((unsigned char)buf[0] == 'h');
        assert((unsigned char)buf[1] == 0xC3);
        assert((unsigned char)buf[2] == 0xA9);
        assert((unsigned char)buf[3] == 0xF0);
        assert((unsigned char)buf[6] == 0x80);
    }

    return 0;
}

/* Native test for metallic's wchar string/memory utilities.
 *
 * We include the metallic .c source files directly into this translation
 * unit (matching the convention used by test/native/math/...).  Each
 * metallic function is renamed via macros so we don't collide with the
 * host libc's definitions.  Calls in this test therefore exercise the
 * metallic implementation, while everything else (printf, assert) uses
 * the host libc. */

#define wmemcmp  m_wmemcmp
#define wmemchr  m_wmemchr
#define wmemset  m_wmemset
#define wmemcpy  m_wmemcpy
#define wmemmove m_wmemmove

#define wcslen   m_wcslen
#define wcscmp   m_wcscmp
#define wcsncmp  m_wcsncmp
#define wcschr   m_wcschr
#define wcsrchr  m_wcsrchr
#define wcscpy   m_wcscpy
#define wcsncpy  m_wcsncpy
#define wcscat   m_wcscat
#define wcsncat  m_wcsncat
#define wcsspn   m_wcsspn
#define wcscspn  m_wcscspn
#define wcspbrk  m_wcspbrk
#define wcsstr   m_wcsstr
#define wcstok   m_wcstok
#define wcscoll  m_wcscoll
#define wcsxfrm  m_wcsxfrm

#include "src/wchar/wmemcmp.c"
#include "src/wchar/wmemchr.c"
#include "src/wchar/wmemset.c"
#include "src/wchar/wmemcpy.c"
#include "src/wchar/wmemmove.c"
#include "src/wchar/wcslen.c"
#include "src/wchar/wcscmp.c"
#include "src/wchar/wcsncmp.c"
#include "src/wchar/wcschr.c"
#include "src/wchar/wcsrchr.c"
#include "src/wchar/wcscpy.c"
#include "src/wchar/wcsncpy.c"
#include "src/wchar/wcscat.c"
#include "src/wchar/wcsncat.c"
#include "src/wchar/wcsspn.c"
#include "src/wchar/wcscspn.c"
#include "src/wchar/wcspbrk.c"
#include "src/wchar/wcsstr.c"
#include "src/wchar/wcstok.c"
#include "src/wchar/wcscoll.c"
#include "src/wchar/wcsxfrm.c"

#include <assert.h>
#include <wchar.h>

int main(void)
{
    /* wcslen */
    assert(m_wcslen(L"") == 0);
    assert(m_wcslen(L"hello") == 5);
    assert(m_wcslen(L"héllo") == 5); /* 5 codepoints regardless of wchar_t width */
    assert(m_wcslen(L"日本語") == 3);

    /* wcscmp / wcsncmp */
    assert(m_wcscmp(L"abc", L"abc") == 0);
    assert(m_wcscmp(L"abc", L"abd") < 0);
    assert(m_wcscmp(L"abd", L"abc") > 0);
    assert(m_wcscmp(L"abc", L"abcd") < 0);
    assert(m_wcsncmp(L"abcX", L"abcY", 3) == 0);
    assert(m_wcsncmp(L"abcX", L"abcY", 4) < 0);
    assert(m_wcsncmp(L"abc", L"abc", 100) == 0);

    /* wcschr / wcsrchr */
    {
        const wchar_t s[] = L"abcaba";
        assert(m_wcschr(s, L'a') == s);
        assert(m_wcschr(s, L'c') == s + 2);
        assert(m_wcschr(s, L'z') == 0);
        assert(m_wcschr(s, L'\0') == s + 6);
        assert(m_wcsrchr(s, L'a') == s + 5);
        assert(m_wcsrchr(s, L'b') == s + 4);
        assert(m_wcsrchr(s, L'z') == 0);
        assert(m_wcsrchr(s, L'\0') == s + 6);
    }

    /* wcscpy / wcsncpy round-trip */
    {
        wchar_t dst[16];
        assert(m_wcscpy(dst, L"héllo") == dst);
        assert(m_wcscmp(dst, L"héllo") == 0);

        for (int i = 0; i < 16; ++i)
            dst[i] = L'X';
        m_wcsncpy(dst, L"abc", 6);
        assert(dst[0] == L'a' && dst[1] == L'b' && dst[2] == L'c');
        assert(dst[3] == 0 && dst[4] == 0 && dst[5] == 0);
        assert(dst[6] == L'X'); /* untouched */

        for (int i = 0; i < 16; ++i)
            dst[i] = L'X';
        m_wcsncpy(dst, L"abcdef", 3);
        assert(dst[0] == L'a' && dst[1] == L'b' && dst[2] == L'c');
        assert(dst[3] == L'X'); /* not NUL-terminated */
    }

    /* wcscat / wcsncat */
    {
        wchar_t buf[32] = L"foo";
        m_wcscat(buf, L"bar");
        assert(m_wcscmp(buf, L"foobar") == 0);

        wchar_t buf2[32] = L"foo";
        m_wcsncat(buf2, L"barbaz", 3);
        assert(m_wcscmp(buf2, L"foobar") == 0);

        wchar_t buf3[32] = L"foo";
        m_wcsncat(buf3, L"b", 5);
        assert(m_wcscmp(buf3, L"foob") == 0);
    }

    /* wcsspn / wcscspn / wcspbrk */
    assert(m_wcsspn(L"aabbcXYZ", L"abc") == 5);
    assert(m_wcsspn(L"XYZ", L"abc") == 0);
    assert(m_wcsspn(L"", L"abc") == 0);
    assert(m_wcscspn(L"XYZabc", L"abc") == 3);
    assert(m_wcscspn(L"abc", L"XYZ") == 3);
    {
        const wchar_t s[] = L"XYZabc";
        assert(m_wcspbrk(s, L"cab") == s + 3);
        assert(m_wcspbrk(s, L"!?") == 0);
    }

    /* wcsstr */
    {
        const wchar_t s[] = L"the quick brown fox";
        assert(m_wcsstr(s, L"") == s);
        assert(m_wcsstr(s, L"quick") == s + 4);
        assert(m_wcsstr(s, L"fox") == s + 16);
        assert(m_wcsstr(s, L"slow") == 0);
        assert(m_wcsstr(s, L"the quick brown fox") == s);
    }

    /* wcstok three-arg state-passing */
    {
        wchar_t input[] = L"  hello, world ,, foo";
        wchar_t* state = 0;
        wchar_t* a = m_wcstok(input, L" ,", &state);
        wchar_t* b = m_wcstok(0, L" ,", &state);
        wchar_t* c = m_wcstok(0, L" ,", &state);
        wchar_t* d = m_wcstok(0, L" ,", &state);
        assert(a && m_wcscmp(a, L"hello") == 0);
        assert(b && m_wcscmp(b, L"world") == 0);
        assert(c && m_wcscmp(c, L"foo") == 0);
        assert(d == 0);

        /* Independent state */
        wchar_t s1[] = L"a,b";
        wchar_t s2[] = L"x,y";
        wchar_t* st1 = 0;
        wchar_t* st2 = 0;
        assert(m_wcscmp(m_wcstok(s1, L",", &st1), L"a") == 0);
        assert(m_wcscmp(m_wcstok(s2, L",", &st2), L"x") == 0);
        assert(m_wcscmp(m_wcstok(0, L",", &st1), L"b") == 0);
        assert(m_wcscmp(m_wcstok(0, L",", &st2), L"y") == 0);
    }

    /* wcscoll == wcscmp in C locale */
    assert(m_wcscoll(L"abc", L"abc") == 0);
    assert(m_wcscoll(L"abc", L"abd") < 0);

    /* wcsxfrm: identity in C locale */
    {
        wchar_t dst[16] = {0};
        size_t n = m_wcsxfrm(dst, L"hello", 16);
        assert(n == 5);
        assert(m_wcscmp(dst, L"hello") == 0);

        size_t n2 = m_wcsxfrm(0, L"hello", 0);
        assert(n2 == 5);

        wchar_t small[3] = {0};
        m_wcsxfrm(small, L"hello", 3);
        assert(small[0] == L'h' && small[1] == L'e' && small[2] == 0);
    }

    /* wmemcmp / wmemchr / wmemset / wmemcpy / wmemmove */
    {
        wchar_t a[] = {1, 2, 3, 4, 5};
        wchar_t b[] = {1, 2, 3, 4, 5};
        wchar_t c[] = {1, 2, 9, 4, 5};
        assert(m_wmemcmp(a, b, 5) == 0);
        assert(m_wmemcmp(a, c, 5) < 0);
        assert(m_wmemcmp(c, a, 5) > 0);

        assert(m_wmemchr(a, 3, 5) == a + 2);
        assert(m_wmemchr(a, 99, 5) == 0);
        /* embedded zero is found, unlike strchr semantics */
        wchar_t z[] = {1, 0, 2};
        assert(m_wmemchr(z, 0, 3) == z + 1);

        wchar_t buf[8];
        m_wmemset(buf, 0xABCD, 8);
        for (int i = 0; i < 8; ++i)
            assert(buf[i] == (wchar_t)0xABCD);

        wchar_t src[] = {10, 20, 30, 40};
        wchar_t dst[4];
        m_wmemcpy(dst, src, 4);
        assert(dst[0] == 10 && dst[1] == 20 && dst[2] == 30 && dst[3] == 40);

        wchar_t overlap[] = {1, 2, 3, 4, 5, 6, 7, 8};
        m_wmemmove(overlap + 2, overlap, 4);
        assert(overlap[0] == 1 && overlap[1] == 2);
        assert(overlap[2] == 1 && overlap[3] == 2);
        assert(overlap[4] == 3 && overlap[5] == 4);
    }

    return 0;
}

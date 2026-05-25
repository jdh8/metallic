#include <assert.h>
#include <wchar.h>

int main(void)
{
    /* wcslen */
    assert(wcslen(L"") == 0);
    assert(wcslen(L"hello") == 5);
    assert(wcslen(L"héllo") == 5);
    assert(wcslen(L"日本語") == 3);

    /* wcscmp / wcsncmp */
    assert(wcscmp(L"abc", L"abc") == 0);
    assert(wcscmp(L"abc", L"abd") < 0);
    assert(wcscmp(L"abd", L"abc") > 0);
    assert(wcscmp(L"abc", L"abcd") < 0);
    assert(wcsncmp(L"abcX", L"abcY", 3) == 0);
    assert(wcsncmp(L"abcX", L"abcY", 4) < 0);

    /* wcschr / wcsrchr */
    {
        const wchar_t s[] = L"abcaba";
        assert(wcschr(s, L'a') == s);
        assert(wcschr(s, L'c') == s + 2);
        assert(wcschr(s, L'z') == 0);
        assert(wcschr(s, L'\0') == s + 6);
        assert(wcsrchr(s, L'a') == s + 5);
        assert(wcsrchr(s, L'b') == s + 4);
        assert(wcsrchr(s, L'z') == 0);
        assert(wcsrchr(s, L'\0') == s + 6);
    }

    /* wcscpy / wcsncpy */
    {
        wchar_t dst[16];
        assert(wcscpy(dst, L"héllo") == dst);
        assert(wcscmp(dst, L"héllo") == 0);

        for (int i = 0; i < 16; ++i)
            dst[i] = L'X';
        wcsncpy(dst, L"abc", 6);
        assert(dst[0] == L'a' && dst[1] == L'b' && dst[2] == L'c');
        assert(dst[3] == 0 && dst[4] == 0 && dst[5] == 0);
        assert(dst[6] == L'X');
    }

    /* wcscat / wcsncat */
    {
        wchar_t buf[32] = L"foo";
        wcscat(buf, L"bar");
        assert(wcscmp(buf, L"foobar") == 0);

        wchar_t buf2[32] = L"foo";
        wcsncat(buf2, L"barbaz", 3);
        assert(wcscmp(buf2, L"foobar") == 0);
    }

    /* wcsspn / wcscspn / wcspbrk */
    assert(wcsspn(L"aabbcXYZ", L"abc") == 5);
    assert(wcscspn(L"XYZabc", L"abc") == 3);
    {
        const wchar_t s[] = L"XYZabc";
        assert(wcspbrk(s, L"cab") == s + 3);
        assert(wcspbrk(s, L"!?") == 0);
    }

    /* wcsstr */
    {
        const wchar_t s[] = L"the quick brown fox";
        assert(wcsstr(s, L"") == s);
        assert(wcsstr(s, L"quick") == s + 4);
        assert(wcsstr(s, L"fox") == s + 16);
        assert(wcsstr(s, L"slow") == 0);
    }

    /* wcstok three-arg state-passing */
    {
        wchar_t input[] = L"  hello, world ,, foo";
        wchar_t* state = 0;
        wchar_t* a = wcstok(input, L" ,", &state);
        wchar_t* b = wcstok(0, L" ,", &state);
        wchar_t* c = wcstok(0, L" ,", &state);
        wchar_t* d = wcstok(0, L" ,", &state);
        assert(a && wcscmp(a, L"hello") == 0);
        assert(b && wcscmp(b, L"world") == 0);
        assert(c && wcscmp(c, L"foo") == 0);
        assert(d == 0);
    }

    /* wcscoll, wcsxfrm */
    assert(wcscoll(L"abc", L"abc") == 0);
    {
        wchar_t dst[16];
        assert(wcsxfrm(dst, L"hello", 16) == 5);
        assert(wcscmp(dst, L"hello") == 0);
    }

    /* wmem* */
    {
        wchar_t a[] = {1, 2, 3, 4, 5};
        wchar_t b[] = {1, 2, 3, 4, 5};
        wchar_t c[] = {1, 2, 9, 4, 5};
        assert(wmemcmp(a, b, 5) == 0);
        assert(wmemcmp(a, c, 5) < 0);

        assert(wmemchr(a, 3, 5) == a + 2);
        assert(wmemchr(a, 99, 5) == 0);

        wchar_t buf[8];
        wmemset(buf, 0xABCD, 8);
        for (int i = 0; i < 8; ++i)
            assert(buf[i] == (wchar_t)0xABCD);

        wchar_t src[] = {10, 20, 30, 40};
        wchar_t dst[4];
        wmemcpy(dst, src, 4);
        assert(dst[0] == 10 && dst[3] == 40);

        wchar_t overlap[] = {1, 2, 3, 4, 5, 6, 7, 8};
        wmemmove(overlap + 2, overlap, 4);
        assert(overlap[2] == 1 && overlap[5] == 4);
    }

    return 0;
}

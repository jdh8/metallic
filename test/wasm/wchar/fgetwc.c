#include <wchar.h>
#include <stdio.h>
#include <assert.h>

int main(void)
{
    FILE* f = fopen("fgetwc.tmp", "w");
    /* "Aこ😀" — 1, 3, 4 bytes UTF-8 = 8 bytes. */
    fputs("A\xE3\x81\x93\xF0\x9F\x98\x80", f);
    fclose(f);

    f = fopen("fgetwc.tmp", "r");

    wint_t c = fgetwc(f);
    assert(c == L'A');

    c = fgetwc(f);
    assert(c == L'こ');

    c = fgetwc(f);
    assert(c == L'\U0001F600');

    c = fgetwc(f);
    assert(c == WEOF);

    /* ungetwc + re-read. */
    clearerr(f);
    rewind(f);
    c = fgetwc(f);
    assert(c == L'A');

    assert(ungetwc(L'A', f) == L'A');
    c = fgetwc(f);
    assert(c == L'A');

    fclose(f);
    remove("fgetwc.tmp");
    return 0;
}

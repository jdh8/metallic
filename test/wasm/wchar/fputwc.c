#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    FILE* f = fopen("fputwc.tmp", "w");
    assert(f);

    assert(fputwc(L'A', f) == L'A');
    assert(fputwc(L'こ', f) == (wint_t)L'こ');
    assert(fputwc(L'\U0001F600', f) == (wint_t)L'\U0001F600');

    fclose(f);

    f = fopen("fputwc.tmp", "r");
    char buf[16] = {0};
    size_t n = fread(buf, 1, sizeof buf, f);
    assert(n == 8);
    assert(memcmp(buf, "A\xE3\x81\x93\xF0\x9F\x98\x80", 8) == 0);

    fclose(f);
    remove("fputwc.tmp");
    return 0;
}

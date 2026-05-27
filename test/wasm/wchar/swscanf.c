#include <wchar.h>
#include <stdio.h>
#include <assert.h>

int main(void)
{
    int a, b;
    float f;
    char s[32];

    int got = swscanf(L"42 -7 3.14 hello", L"%d %d %f %s", &a, &b, &f, s);
    assert(got == 4);
    assert(a == 42);
    assert(b == -7);
    assert(f > 3.13f && f < 3.15f);
    assert(s[0] == 'h' && s[1] == 'e');

    /* Hex. */
    unsigned x;
    got = swscanf(L"0xCAFEBABE", L"%x", &x);
    assert(got == 1);
    assert(x == 0xCAFEBABE);

    return 0;
}

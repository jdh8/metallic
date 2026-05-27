#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    int a, b;
    unsigned x;
    char buf[32];

    int got = sscanf("  42 -17 0xCAFEBABE", "%d %d %x", &a, &b, &x);
    assert(got == 3);
    assert(a == 42);
    assert(b == -17);
    assert(x == 0xCAFEBABE);

    /* %c without width: exactly 1 char. */
    char c;
    got = sscanf("xyz", "%c%c%c", &c, &buf[0], &buf[1]);
    assert(got == 3);
    assert(c == 'x');
    assert(buf[0] == 'y');
    assert(buf[1] == 'z');

    /* Scanset. */
    got = sscanf("hello world!", "%[a-z]", buf);
    assert(got == 1);
    assert(strcmp(buf, "hello") == 0);

    /* Negated scanset. */
    got = sscanf("abc:def", "%[^:]", buf);
    assert(got == 1);
    assert(strcmp(buf, "abc") == 0);

    /* Suppression. */
    got = sscanf("123 456", "%*d %d", &a);
    assert(got == 1);
    assert(a == 456);

    /* Length modifier. */
    long long ll;
    got = sscanf("9999999999", "%lld", &ll);
    assert(got == 1);
    assert(ll == 9999999999LL);

    /* %n records bytes consumed and does NOT count as conversion. */
    int n;
    got = sscanf("abc xyz", "%s%n", buf, &n);
    assert(got == 1);
    assert(n == 3);
    assert(strcmp(buf, "abc") == 0);

    /* Width caps. */
    got = sscanf("abcdefg", "%3s", buf);
    assert(got == 1);
    assert(strcmp(buf, "abc") == 0);

    /* Octal. */
    got = sscanf("0777", "%o", &x);
    assert(got == 1);
    assert(x == 0777);

    /* %i auto-detects base. */
    got = sscanf("0x10 010 10", "%i %i %i", &a, &b, &(int){0});
    (void)got;
    assert(a == 16);
    assert(b == 8);

    return 0;
}

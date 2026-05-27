#include <string.h>
#include <assert.h>

int main(void)
{
    assert(strcoll("abc", "abc") == 0);
    assert(strcoll("abc", "abd") < 0);
    assert(strcoll("abd", "abc") > 0);

    char buf[16];
    size_t n = strxfrm(buf, "hello", sizeof buf);
    assert(n == 5);
    assert(strcmp(buf, "hello") == 0);

    /* Truncation: returns full length, copies n-1 bytes + NUL. */
    n = strxfrm(buf, "abcdefgh", 4);
    assert(n == 8);
    assert(strcmp(buf, "abc") == 0);

    /* n == 0: dst may be ignored. */
    n = strxfrm((void*)0, "hi", 0);
    assert(n == 2);

    return 0;
}

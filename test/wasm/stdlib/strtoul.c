#include <assert.h>
#include <stdlib.h>
#include <errno.h>

#define RUN(s, junk, base) check(s junk, base, sizeof(s) - 1)
#define CANONICAL(x, junk) assert(RUN(#x, junk, 0) == x)

#define OVERFLOW(...) do {                   \
    errno = 0;                               \
    assert(RUN(__VA_ARGS__) == -1); \
    assert(errno == ERANGE);        \
} while (0)

static unsigned long check(const char s[static 1], int base, long length)
{
    char* end;
    unsigned long x = strtoul(s, &end, base);
    assert(end == s + length);
    return x;
}

int main(void)
{
    CANONICAL( +24680, "$foo");
    CANONICAL(  0xdeadbeef, "gf");
    CANONICAL(   -0232776532, "9");
    CANONICAL(0, "x");

    assert(RUN("", "jdh8", 0) == 0);
    assert(RUN("", "jdh8", 19) == 0);
    assert(RUN("jdh8", "#1993", 20) == 157548);
    assert(RUN("\tjdh8", "", 25) == 305433);
    assert(RUN("\n+jdh8", "whatever", 30) == 525218);
    assert(RUN(" -jdh8", "", 36) == -903932);

    assert(RUN("3vvvvvv", "...", 32) == -1);
    assert(RUN("-3vvvvvv", "...", 32) == 1);

    assert(!errno);

    OVERFLOW("-321f5a16ga5s1g65as05vs", "", 36);
    OVERFLOW("154165asjndiniasndi3sd", "", 36);
}

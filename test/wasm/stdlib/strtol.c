#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#define RUN(s, junk, base) check(s junk, base, sizeof(s) - 1)
#define CANONICAL(x, junk) assert(RUN(#x, junk, 0) == x)

#define OVERFLOW(condition) do {      \
    errno = 0;                        \
    assert(condition);       \
    assert(errno == ERANGE); \
} while (0)

static long check(const char s[static 1], int base, long length)
{
    char* end;
    long x = strtol(s, &end, base);
    assert(end == s + length);
    return x;
}

int main(void)
{
    CANONICAL( +13579, "$foo");
    CANONICAL(  0x000fdead, "gf");
    CANONICAL(   -0232776532, "9");

    CANONICAL(2147483647, "junk");
    CANONICAL(-2147483648, "junk");
    CANONICAL(0, "x");

    assert(RUN("", "jdh8", 0) == 0);
    assert(RUN("", "jdh8", 19) == 0);
    assert(RUN("jdh8", "#1993", 20) == 157548);
    assert(RUN("\tjdh8", "", 25) == 305433);
    assert(RUN("\n+jdh8", "whatever", 30) == 525218);
    assert(RUN(" -jdh8", "", 36) == -903932);

    assert(RUN("553032005531", "666", 6) == INT32_MAX);
    assert(RUN("-553032005532", "666", 6) == INT32_MIN);

    assert(RUN("zik0zj", "...", 36) == INT32_MAX);
    assert(RUN("-zik0zk", "...", 36) == INT32_MIN);

    assert(!errno);

    OVERFLOW(RUN("-321f5a16ga5s1g65as05vs", "", 36) == LONG_MIN);
    OVERFLOW(RUN("154165asjndiniasndi3sd", "", 36) == LONG_MAX);
}

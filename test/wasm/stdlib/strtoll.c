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

static long long check(const char s[static 1], int base, long length)
{
    char* end;
    long long x = strtoll(s, &end, base);
    assert(end == s + length);
    return x;
}

int main(void)
{
    CANONICAL( +314159265358979, "$foo");
    CANONICAL(  0x2018deadbeef1337, "gf");
    CANONICAL(   -0232776532567256776, "9");

    CANONICAL(9223372036854775807, "junk");
    CANONICAL(-9223372036854775807, "junk");
    CANONICAL(0, "x");

    assert(RUN("", "owm111", 0) == 0);
    assert(RUN("", "owm111", 24) == 0);
    assert(RUN("ChenPangHe", "#1993", 30) == 247673263235924);
    assert(RUN("ChenPangHe", "", 32) == 441410745254446);
    assert(RUN("+ChenPangHe", "zzzzz", 34) == 759734417156424);
    assert(RUN("-ChenPangHe", "", 36) == -1267827042283826);

    assert(RUN("64ie1focnn5g77", "www", 25) == INT64_MAX);
    assert(RUN("-64ie1focnn5g78", "www", 25) == INT64_MIN);

    assert(RUN("1y2p0ij32e8e7", "...", 36) == INT64_MAX);
    assert(RUN("-1y2p0ij32e8e8", "...", 36) == INT64_MIN);

    assert(!errno);

    OVERFLOW(RUN("-321f5a16ga5s1g65as05vs", "", 36) == LLONG_MIN);
    OVERFLOW(RUN("154165asjndiniasndi3sd", "", 36) == LLONG_MAX);
}

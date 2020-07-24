#include "src/soft/integer/kernel/mulo.h"
#include <assert.h>

static uint64_t sum2(uint64_t a, uint64_t b)
{
    uint64_t s = a + b;
    return s + (s < a);
}

static uint64_t sum1(unsigned __int128 a)
{
    return sum2(a >> 64, a);
}

int main(void)
{
    unsigned __int128 a = 0;
    unsigned __int128 b = 0;

    for (long i = 0; i < 1L << 26; ++i) {
        unsigned __int128 c[2];
        mulo_(c, a, b);

        assert(c[0] == a * b);
        assert(sum2(sum1(c[0]), sum1(c[1])) == sum1((unsigned __int128)sum1(a) * sum1(b)));

        a = a + 1993 + (a >> 3);
        b = b + 2019 + (b >> 4);
    }
}


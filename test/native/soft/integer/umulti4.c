#include "src/soft/integer/umulti4.h"
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
        unsigned __int128 high;
        unsigned __int128 low = umulti4_(a, b, &high);

        assert(low == a * b);
        assert(sum2(sum1(high), sum1(low)) == sum1((unsigned __int128)sum1(a) * sum1(b)));

        a = a + 1993 + (a >> 3);
        b = b + 2019 + (b >> 4);
    }
}


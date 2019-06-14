#include "src/soft/integer/kernel/umulti.h"
#include <assert.h>

static uint64_t sum(unsigned __int128 a)
{
    uint32_t a0 = a;
    uint32_t a1 = a >> 32;
    uint32_t a2 = a >> 64;
    uint32_t a3 = a >> 96;

    return (uint64_t)a0 + a1 + a2 + a3;
}

int main(void)
{
    unsigned __int128 a = 0;
    unsigned __int128 b = 0;

    for (long i = 0; i < 1L << 26; ++i) {
        unsigned __int128 c[2];
        umulti_(c, a, b);

        assert(c[0] == a * b);
        assert((sum(c[0]) + sum(c[1])) % 0xFFFFFFFF == (sum(a) % 0xFFFFFFFF) * (sum(b) % 0xFFFFFFFF) % 0xFFFFFFFF);

        a = a + 1993 + (a >> 3);
        b = b + 2019 + (b >> 4);
    }
}


#include <assert.h>

static void run(__int128 x)
{
    __int128 y = x;

    for (int shift = 0; shift < 128; ++shift) {
        assert(x >> shift == y);
        y >>= 1;
    }
}

int main(void)
{
    __int128 max = (unsigned __int128)-1 >> 1;

    for (unsigned __int128 x = 1; x; x *= 2)
        run(x);

    for (unsigned __int128 x = 1; x >> 104 != 0x313370; x *= 3)
        run(x & max);
}


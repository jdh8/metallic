#include <assert.h>

static void run(unsigned __int128 x)
{
    unsigned __int128 y = x;

    for (int shift = 0; shift < 128; ++shift) {
        assert(x << shift == y);
        y <<= 1;
    }
}

int main(void)
{
    for (unsigned __int128 x = 1; x; x *= 2)
        run(x);

    for (unsigned __int128 x = 1; x >> 104 != 0x313370; x *= 3)
        run(x);
}


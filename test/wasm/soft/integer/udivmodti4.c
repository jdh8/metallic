#include "src/soft/integer/udivmodti4.h"
#include <assert.h>

static void run(unsigned __int128 a, unsigned __int128 b)
{
    unsigned __int128 r;
    unsigned __int128 q = udivmodti4_(a, b, &r);

    assert(a == b * q + r);
}

int main(void)
{
    for (unsigned __int128 a = 1; a >> 116 != 0xFFF; a *= 3)
        for (unsigned __int128 b = 1; b >> 116 != 0xFFF; b *= 5)
            run(a, b);
}



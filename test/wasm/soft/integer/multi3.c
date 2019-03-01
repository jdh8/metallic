#include <assert.h>
#include <stdint.h>

unsigned __int128 __multi3(unsigned __int128 a, unsigned __int128 b);

static void run2(unsigned __int128 a, unsigned __int128 b)
{
    unsigned __int128 c = __multi3(a, b);

    assert((uint64_t)a * (uint64_t)b == (uint64_t)c);
    assert(b * a == c);
    assert(-a * b == -c);
    assert(a * -b == -c);
    assert(-a * -b == c);
}

static void run3(unsigned __int128 a, unsigned __int128 b, unsigned __int128 c)
{
    assert(__multi3(a, b + c) == a * b + a * c);
    assert(a * __multi3(b, c) == a * b * c);
}

int main(void)
{
    for (unsigned __int128 a = 1; a >> 116 != 0xFFF; a *= 3)
        for (unsigned __int128 b = 1; b >> 116 != 0xFFF; b *= 5)
            run2(a, b);

    for (unsigned __int128 a = 1; a >> 120 != 0xFF; a *= 3)
        for (unsigned __int128 b = 1; b >> 120 != 0xFF; b *= 5)
            for (unsigned __int128 c = 1; c >> 120 != 0xFF; c *= 7)
                run3(a, b, c);
}



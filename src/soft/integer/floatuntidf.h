#include "clzti2.h"
#include "../../math/reinterpret.h"
#include <stdint.h>

static double _floatuntidf(unsigned __int128 a)
{
    const unsigned __int128 bit = (unsigned __int128)1 << 74;
    const unsigned __int128 mask = 3 * bit - 1;

    if (!a)
        return 0;

    int space = _clzti2(a);
    unsigned __int128 normalized = a << space;
    uint64_t adjustment = normalized & bit && normalized & mask;
    uint64_t significand = normalized >> 75 & 0x000FFFFFFFFFFFFF;
    uint64_t exp = 1023 + 127 - space;

    return reinterpret(double, (significand | exp << 52) + adjustment);
}

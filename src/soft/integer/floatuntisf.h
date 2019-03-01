#include "clzti2.h"
#include "../../math/reinterpret.h"
#include <stdint.h>

static float _floatuntisf(unsigned __int128 a)
{
    const unsigned __int128 bit = (unsigned __int128)1 << 103;
    const unsigned __int128 mask = 3 * bit - 1;

    if (!a)
        return 0;

    int space = _clzti2(a);
    unsigned __int128 normalized = a << space;
    uint32_t adjustment = normalized & bit && normalized & mask;
    uint32_t significand = normalized >> 104 & 0x7FFFFF;
    uint32_t exp = 127 + 127 - space;

    return reinterpret(float, (significand | exp << 23) + adjustment);
}

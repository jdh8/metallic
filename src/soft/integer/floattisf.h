#include "clzti2.h"
#include "../../math/reinterpret.h"
#include <stdint.h>

static float _floattisf(__int128 a)
{
    const unsigned __int128 bit = (unsigned __int128)1 << 103;
    const unsigned __int128 mask = 3 * bit - 1;

    if (!a)
        return 0;

    unsigned __int128 magnitude = a < 0 ? -a : a;

    int space = _clzti2(magnitude);
    unsigned __int128 normalized = magnitude << space;
    uint32_t adjustment = normalized & bit && normalized & mask;
    uint32_t significand = normalized >> 104 & 0x007FFFFF;
    uint32_t exp = 127 + 127 - space;
    uint32_t sign = a >> 96 & 0x80000000;

    return reinterpret(float, (significand | exp << 23 | sign) + adjustment);
}

#include "clzti2.h"
#include "../../math/reinterpret.h"
#include <stdint.h>

static double floattidf_(__int128 a)
{
    const unsigned __int128 bit = (unsigned __int128)1 << 74;
    const unsigned __int128 mask = 3 * bit - 1;

    if (!a)
        return 0;

    unsigned __int128 magnitude = a < 0 ? -a : a;

    int space = clzti2_(magnitude);
    unsigned __int128 normalized = magnitude << space;
    uint64_t adjustment = normalized & bit && normalized & mask;
    uint64_t significand = normalized >> 75 & 0x000FFFFFFFFFFFFF;
    uint64_t exp = 1023 + 127 - space;
    uint64_t sign = a >> 64 & 0x8000000000000000;

    return reinterpret(double, (significand | exp << 52 | sign) + adjustment);
}


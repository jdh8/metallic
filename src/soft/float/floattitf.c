#include "../integer/clzti2.h"
#include "../../math/reinterpret.h"

long double __floattitf(__int128 a)
{
    const unsigned __int128 mask = (unsigned __int128)-1 >> 16;

    if (!a)
        return 0;

    unsigned __int128 sign = a & (unsigned __int128)1 << 127;
    unsigned __int128 magnitude = a < 0 ? -a : a;

    int space = _clzti2(magnitude);
    unsigned __int128 normalized = magnitude << space;
    unsigned __int128 adjustment = normalized & 0x4000 && normalized & 0xBFFF;
    unsigned __int128 significand = normalized >> 15 & mask;
    unsigned __int128 exp = 0x3FFF + 127 - space;

    return reinterpret(long double, (significand | exp << 112 | sign) + adjustment);
}

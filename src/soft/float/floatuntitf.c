#include "../integer/clzti2.h"
#include "../../math/reinterpret.h"

long double __floatuntitf(unsigned __int128 a)
{
    const unsigned __int128 mask = (unsigned __int128)-1 >> 16;

    if (!a)
        return 0;

    int space = clzti2_(a);
    unsigned __int128 normalized = a << space;
    unsigned __int128 adjustment = normalized & 0x4000 && normalized & 0xBFFF;
    unsigned __int128 significand = normalized >> 15 & mask;
    unsigned __int128 exp = 0x3FFF + 127 - space;

    return reinterpret(long double, (significand | exp << 112) + adjustment);
}

#include "../reinterpret.h"

static int maskshift_(int exp)
{
    if (exp < 0x3FFF)
        return 127;

    int shift = 112 + 0x3FFF - exp;
    return shift < 0 ? 0 : shift;
}

long double truncl(long double x)
{
    const unsigned __int128 max = -1;
    unsigned __int128 bits = reinterpret(unsigned __int128, x);
    unsigned __int128 mask = max << maskshift_(bits << 1 >> 113);

    return reinterpret(long double, bits & mask);
}

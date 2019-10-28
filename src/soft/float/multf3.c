#include "../../math/reinterpret.h"

static unsigned __int128 subnormal_(unsigned __int128 a, unsigned __int128 b)
{
}

static unsigned __int128 normal_(unsigned __int128 a, unsigned __int128 b)
{
}

static unsigned __int128 magnitude_(unsigned __int128 a, unsigned __int128 b)
{
    const unsigned __int128 inf = (unsigned __int128)0x7FFF << 112;
    const unsigned __int128 min = (unsigned __int128)1 << 112;

    if (a > inf)
        return a;

    if (b >= inf)
        return b;

    if (a == inf)
        return a;

    if (a < min)
        return subnormal_(a, b);

    if (b < min)
        return subnormal_(b, a);

    return normal_(a, b);
}

long double __multf3(long double x, long double y)
{
    const unsigned __int128 sign = (unsigned __int128)1 << 127;
    unsigned __int128 a = reinterpret(unsigned __int128, x);
    unsigned __int128 b = reinterpret(unsigned __int128, y);

    return reinterpret(long double, magnitude_(a & ~sign, b & ~sign) | ((a ^ b) & sign));
}

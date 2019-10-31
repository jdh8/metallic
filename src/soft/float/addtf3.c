#include "../../math/reinterpret.h"

static __int128 add_(__int128 a, __int128 b);
static __int128 sub_(__int128 a, __int128 b);

static unsigned __int128 sorted_(unsigned __int128 a, unsigned __int128 b)
{
    const unsigned __int128 inf = (unsigned __int128)0x7FFF << 112;
    const unsigned __int128 sign = (unsigned __int128)1 << 127;

    unsigned __int128 aa = a & ~sign;
    unsigned __int128 bb = b & ~sign;

    if (aa > inf)
        return a | (unsigned __int128)1 << 111;

    if ((a ^ b) >> 127) {
        if (aa == inf)
            return a | (unsigned __int128)(bb == inf) << 111;

        if (aa == bb)
            return 0;

        return (a & sign) | sub_(aa, bb);
    }

    return (a & sign) | add_(aa, bb);
}

long double __addtf3(long double x, long double y)
{
    const unsigned __int128 sign = (unsigned __int128)1 << 127;
    unsigned __int128 a = reinterpret(unsigned __int128, x);
    unsigned __int128 b = reinterpret(unsigned __int128, y);

    return reinterpret(long double, (a & ~sign) < (b & ~sign) ? sorted_(b, a) : sorted_(a, b));
}

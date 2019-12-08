#include "normalizel.h"
#include "../reinterpret.h"

static __int128 finite_(__int128 a);

long double sqrtl(long double x)
{
    const unsigned __int128 inf = (unsigned __int128)0x7FFF << 112;
    const unsigned __int128 nan = inf | (unsigned __int128)1 << 111;

    unsigned __int128 a = reinterpret(unsigned __int128, x);

    if (!(a << 1) || a == inf)
        return x;

    if (a < inf)
        return reinterpret(long double, finite_(normalizel_(a)));

    return reinterpret(long double, a | nan);
}

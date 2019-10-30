#include "../../math/reinterpret.h"

static unsigned __int128 sorted_(unsigned __int128, unsigned __int128);

long double __addtf3(long double x, long double y)
{
    const unsigned __int128 sign = (unsigned __int128)1 << 127;
    unsigned __int128 a = reinterpret(unsigned __int128, x);
    unsigned __int128 b = reinterpret(unsigned __int128, y);

    return reinterpret(long double, (a & ~sign) < (b & ~sign) ? sorted_(b, a) : sorted_(a, b));
}

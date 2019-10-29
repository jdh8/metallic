#include "../integer/kernel/umulti.h"
#include "../../math/long-double/normalizel.h"
#include "../../math/reinterpret.h"
#include <stdint.h>

static unsigned __int128 compose_(int exp, unsigned __int128 significand)
{
    if (exp >= 0x7FFF)
        return (unsigned __int128)0x7FFF << 112;

    if (exp > 0) {
        unsigned __int128 tail = significand << 1;
        return ((unsigned __int128)exp << 112 | tail >> 16) + (((tail & 0xFFFF) | (tail >> 15 & 1)) > 0x8000);
    }

    if (exp >= -112) {
        unsigned __int128 q = significand >> 16 >> -exp;
        unsigned __int128 r = significand << (112 + exp);
        return q + ((r >> 64 | (q & 1 || r & UINT64_MAX)) > 0x8000000000000000);
    }

    return 0;
}

static unsigned __int128 kernel_(__int128 a, __int128 b)
{
    const unsigned __int128 msb = (unsigned __int128)1 << 127;

    unsigned __int128 x = a << 15 | msb;
    unsigned __int128 y = b << 15 | msb;
    unsigned __int128 z[2];

    umulti_(z, x, y);

    _Bool carry = z[1] >> 127;

    return compose_((a >> 112) + (b >> 112) - 0x3FFF + carry, (z[1] | !!z[0]) << carry);
}

static unsigned __int128 magnitude_(unsigned __int128 a, unsigned __int128 b)
{
    const unsigned __int128 inf = (unsigned __int128)0x7FFF << 112;

    if (a >= inf)
        return a | (unsigned __int128)(b - 1 >= inf) << 111;

    if (b >= inf)
        return b | (unsigned __int128)!a << 111;

    if (a && b)
        return kernel_(normalizel_(a), normalizel_(b));

    return 0;
}

long double __multf3(long double x, long double y)
{
    const unsigned __int128 sign = (unsigned __int128)1 << 127;
    unsigned __int128 a = reinterpret(unsigned __int128, x);
    unsigned __int128 b = reinterpret(unsigned __int128, y);

    return reinterpret(long double, magnitude_(a & ~sign, b & ~sign) | ((a ^ b) & sign));
}

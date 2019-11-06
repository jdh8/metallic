#include "../../math/reinterpret.h"

static unsigned __int128 compose_sum_(unsigned __int128 significand, unsigned shift)
{
    if (shift < 0x7FFE)
        return significand + ((unsigned __int128)shift << 112);

    return (unsigned __int128)0x7FFF << 112;
}

static unsigned __int128 add_(unsigned __int128 a, unsigned __int128 b)
{
    const unsigned __int128 implied = (unsigned __int128)1 << 112;

    if (a < implied)
        return a + b;

    unsigned shift = (a >> 112) - (b >> 112 | !(b >> 112));

    if (shift >= 114)
        return a;

    unsigned __int128 aa = (a & (implied - 1)) | implied;
    unsigned __int128 bb = (b & (implied - 1)) | (unsigned __int128)!!(b >> 112) << 112;
    unsigned __int128 i = aa + (bb >> shift);
    unsigned __int128 frac = bb << (128 - shift);

    if (i < implied << 1)
        return compose_sum_(i + ((frac >> 64 | (i & 1 || frac & 0xFFFFFFFFFFFFFFFF)) > 0x8000000000000000), (a >> 112) - 1);

    return compose_sum_((i >> 1) + (i & 1 && (i & 2 || frac)), a >> 112);
}

static unsigned __int128 sub_(unsigned __int128 a, unsigned __int128 b)
{
    if (a < (unsigned __int128)2 << 112)
        return a - b;

    unsigned shift = (a >> 112) - (b >> 112 | !(b >> 112));

    if (shift >= 114)
        return a;

    unsigned __int128 aa = a << 15 | (unsigned __int128)1 << 127;
    unsigned __int128 bb = b << 15 | (unsigned __int128)!!(b >> 112) << 127;
    unsigned __int128 i = aa - (bb >> shift | !!(bb << (128 - shift)));
    __int128 bits = reinterpret(__int128, (long double)i) + (((__int128)(a >> 112) - 0x407E) << 112);

    if (bits >= (__int128)1 << 112)
        return bits;

    return a >> 112 < 16 ? i >> (16 - (a >> 112)) : i << ((a >> 112) - 16);
}

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

long double __subtf3(long double x, long double y)
{
    return __addtf3(x, -y);
}

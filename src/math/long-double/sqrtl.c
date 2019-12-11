#include "normalizel.h"
#include "../reinterpret.h"
#include "../../soft/integer/kernel/umulti.h"
#include <stdint.h>

double sqrt(double);

static uint64_t iterate_(uint64_t x, uint64_t a)
{
    return umuldi_(x, 0xC000000000000000 - (uint64_t)(umuldi_(x, umuldi_(x, a) >> 64) >> 64)) >> 63;
}

static unsigned __int128 fixmul_(uint64_t a, unsigned __int128 b)
{
    return umuldi_(a, b >> 64) + (umuldi_(a, b) >> 64);
}

static unsigned __int128 expand_(uint64_t x, unsigned __int128 a)
{
    return fixmul_(x, ((unsigned __int128)3 << 126) - fixmul_(x, fixmul_(x, a)));
}

static __int128 finite_(__int128 a)
{
    unsigned __int128 significand = (a << 15 | (unsigned __int128)1 << 127) >> (a >> 112 & 1);
    uint64_t initial = (uint64_t)(0x1p90 / sqrt(significand >> 64)) << 5;
    uint64_t estimate = iterate_(initial, significand >> 64) - 1;
    unsigned __int128 reciprocal = expand_(estimate, significand) - 1;
    unsigned __int128 product[2];

    umulti_(product, significand, reciprocal);

    unsigned __int128 root = product[1] >> 13;

    umulti_(product, root << 13, (root + 1) << 13);

    return (((a >> 112) + 0x3FFD) >> 1 << 112 | (product[1] < significand >> 4)) + root;
}

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

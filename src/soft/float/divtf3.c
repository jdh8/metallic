#include "compose/product.h"
#include "../integer/umulti4.h"
#include "../../math/long-double/normalizel.h"
#include "../../math/reinterpret.h"
#include <stdint.h>

static unsigned __int128 fixmul_(uint64_t a, unsigned __int128 b)
{
    return umulditi3_(a, b >> 64) + (umulditi3_(a, b) >> 64);
}

static unsigned __int128 fixdiv_(unsigned __int128 a, unsigned __int128 b)
{
    uint64_t initial = (uint64_t)(0x1p120 / (uint64_t)(b >> 64)) << 7;
    uint64_t estimate = (umulditi3_(initial, -(uint64_t)(umulditi3_(initial, b >> 64) >> 64)) >> 63) - 1;
    unsigned __int128 high;

    umulti4_(a, fixmul_(estimate, -fixmul_(estimate, b)) - 2, &high);

    unsigned __int128 q = high >> 2;
    unsigned __int128 low = umulti4_(q + 1, b, &high);

    return (q + (high >= a >> 4)) << 3 | !!low;
}

static unsigned __int128 kernel_(__int128 a, __int128 b)
{
    const unsigned __int128 msb = (unsigned __int128)1 << 127;
    unsigned __int128 quotient = fixdiv_(a << 15 | msb, b << 15 | msb);
    _Bool carry = quotient >> 127;

    return compose_product_((a >> 112) - (b >> 112) + 0x3FFE + carry, quotient << !carry);
}

static unsigned __int128 magnitude_(unsigned __int128 a, unsigned __int128 b)
{
    const unsigned __int128 inf = (unsigned __int128)0x7FFF << 112;

    if (a >= inf)
        return a | (unsigned __int128)(b >= inf) << 111;

    if (!b)
        return inf | (unsigned __int128)!a << 111;

    if (b > inf)
        return b | (unsigned __int128)1 << 111;

    if (!a || b == inf)
        return 0;

    return kernel_(normalizel_(a), normalizel_(b));
}

long double __divtf3(long double x, long double y)
{
    const unsigned __int128 sign = (unsigned __int128)1 << 127;
    unsigned __int128 a = reinterpret(unsigned __int128, x);
    unsigned __int128 b = reinterpret(unsigned __int128, y);

    return reinterpret(long double, magnitude_(a & ~sign, b & ~sign) | ((a ^ b) & sign));
}

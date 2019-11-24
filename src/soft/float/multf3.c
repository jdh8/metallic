#include "compose/product.h"
#include "../integer/kernel/umulti.h"
#include "../../math/long-double/normalizel.h"
#include "../../math/reinterpret.h"

static unsigned __int128 kernel_(__int128 a, __int128 b)
{
    const unsigned __int128 msb = (unsigned __int128)1 << 127;

    unsigned __int128 x = a << 15 | msb;
    unsigned __int128 y = b << 15 | msb;
    unsigned __int128 z[2];

    umulti_(z, x, y);

    _Bool carry = z[1] >> 127;

    return compose_product_((a >> 112) + (b >> 112) - 0x3FFF + carry, (z[1] | !!z[0]) << !carry);
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

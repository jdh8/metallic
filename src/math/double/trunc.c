#include "../reinterpret.h"
#include "../rounding.h"
#include <stdint.h>

static double trunc_(double x)
{
    const int64_t mask = 0xFFF0000000000000;
    int64_t bits = reinterpret(int64_t, x);
    int64_t magnitude = bits & 0x7FFFFFFFFFFFFFFF;

    if (magnitude < 0x3FF0000000000000) /* 1 */
        return reinterpret(double, bits & 0x8000000000000000);

    if (magnitude < 0x4330000000000000) /* 0x1p52 */
        return reinterpret(double, mask >> ((magnitude >> 52) - 1023) & bits);

    return x;
}

double trunc(double x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_trunc(x);
#endif
    return trunc_(x);
}

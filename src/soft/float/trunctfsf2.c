#include "../../math/reinterpret.h"
#include <math.h>
#include <stdint.h>

static uint64_t magnitude_(uint64_t i)
{
    if (i > 0x7FFF000000000000)
        return i;

    if (i >= 0x43FF000000000000)
        return 0x7FF0000000000000;

    if (i > 0x3C00000000000000)
        return (i - 0x3C00000000000000) << 4;

    return 0;
}

float __trunctfsf2(long double x)
{
    unsigned __int128 bits = reinterpret(unsigned __int128, x);
    uint64_t i = bits >> 64 | !!(bits & UINT64_MAX);

    return reinterpret(double, magnitude_(i & INT64_MAX) | (i & 0x8000000000000000));
}

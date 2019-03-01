#include "../../math/reinterpret.h"
#include <stdint.h>

long double __floatunditf(uint64_t a)
{
    if (!a)
        return 0;

    int space = __builtin_clzll(a);
    unsigned __int128 significand = (unsigned __int128)(a << space & INT64_MAX) << 49;
    unsigned __int128 exp = 0x3FFF + 63 - space;

    return reinterpret(long double, significand | exp << 112);
}

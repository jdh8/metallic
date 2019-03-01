#include "../../math/reinterpret.h"
#include <stdint.h>
#include <stdlib.h>

long double __floatditf(int64_t a)
{
    if (!a)
        return 0;

    uint64_t sign = a & 0x8000000000000000;
    uint64_t magnitude = llabs(a);

    int space = __builtin_clzll(magnitude);
    unsigned __int128 significand = (unsigned __int128)(magnitude << space & INT64_MAX) << 49;
    unsigned __int128 exp = 0x3FFF + 63 - space;

    return reinterpret(long double, significand | exp << 112 | (unsigned __int128)sign << 64);
}

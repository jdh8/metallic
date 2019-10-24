#include "../../math/reinterpret.h"
#include <stdint.h>

static uint64_t magnitude_(uint64_t high, uint64_t low)
{
    if (high > 0x7FFF000000000000 || (high == 0x7FFF000000000000 && low))
        return high;

    if (high >= 0x43FF000000000000)
        return 0x7FF0000000000000;

    if (high >= 0x3C01000000000000)
        return ((high - 0x3C00000000000000) << 4 | low >> 60) + (((low >> 59 & 1) | low << 4) > 0x8000000000000000);

    if (high >= 0x3BCC000000000000) {
        double significand = reinterpret(double, (0x8000000000000000 | high << 15) >> 11 | low >> 60 | !!(low << 4));
        double coeff = reinterpret(double, ((high >> 48) - 0x3802) << 52);
        return reinterpret(uint64_t, significand * coeff);
    }
    return 0;
}

double __trunctfdf2(long double x)
{
    unsigned __int128 i = reinterpret(unsigned __int128, x);
    uint64_t high = i >> 64;

    return reinterpret(double, magnitude_(high & INT64_MAX, i) | (high & 0x8000000000000000));
}

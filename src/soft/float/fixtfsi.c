#include "../../math/reinterpret.h"
#include <stdint.h>

int32_t __fixtfsi(long double x)
{
    uint64_t high = reinterpret(unsigned __int128, x) >> 64;

    if ((high & INT64_MAX) < 0x3FFF000000000000)
        return 0;

    uint64_t sign = high & 0x8000000000000000;
    uint64_t magnitude = (high - 0x3C00000000000000) << 4;

    return reinterpret(double, sign | magnitude);
}

#include "../../math/reinterpret.h"
#include <stdint.h>

uint32_t __fixunstfsi(long double x)
{
    int64_t high = reinterpret(__int128, x) >> 64;

    if (high < 0x3FFF000000000000)
        return 0;

    return reinterpret(double, (high - 0x3C00000000000000) << 4);
}

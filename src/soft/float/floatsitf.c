#include "../../math/reinterpret.h"
#include <math.h>
#include <stdint.h>

long double __floatsitf(int32_t a)
{
    if (!a)
        return 0;

    uint64_t sign = (uint64_t)a & 0x8000000000000000;
    uint64_t magnitude = (reinterpret(uint64_t, fabs((double)a)) >> 4) + 0x3C00000000000000;

    return reinterpret(long double, (unsigned __int128)(sign | magnitude) << 64);
}

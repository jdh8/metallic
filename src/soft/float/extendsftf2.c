#include "../../math/reinterpret.h"
#include <math.h>
#include <stdint.h>

static uint64_t magnitude_(uint64_t i)
{
    if (i >= 0x7FF0000000000000)
        return 0x7800000000000000 | i >> 4;

    return i ? 0x3C00000000000000 + (i >> 4) : 0;
}

long double __extendsftf2(float x)
{
    uint64_t i = reinterpret(uint64_t, (double)x);
    uint64_t high = magnitude_(i & INT64_MAX) | (i & 0x8000000000000000);

    return reinterpret(long double, (unsigned __int128)high << 64);
}

#include "../reinterpret.h"
#include <stdint.h>

float fmaf(float x, float y, float a)
{
    double xy = (double)x * y;
    double s = xy + a;
    uint64_t i = reinterpret(uint64_t, s);

    if ((i & 0x1FFFFFFF) != 0x10000000 || s - s || s - xy == a)
        return s;

    uint64_t adjustment = 1 - ((i ^ reinterpret(uint64_t, s - xy - a)) >> 63 << 1);
    return reinterpret(double, i + adjustment);
}

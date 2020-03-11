#include "../reinterpret.h"
#include <stdint.h>

float fmaf(float a, float b, float c)
{
    double ab = (double)a * b;
    double s = ab + c;
    uint64_t i = reinterpret(uint64_t, s);

    if ((i & 0x1FFFFFFF) != 0x10000000 || s - s || s - ab == c)
        return s;

    uint64_t adjustment = 1 - ((i ^ reinterpret(uint64_t, s - ab - c)) >> 63 << 1);
    return reinterpret(double, i + adjustment);
}

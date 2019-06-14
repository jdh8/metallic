#include "normalizef.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

float frexpf(float x, int exp[static 1])
{
    int32_t i = reinterpret(int32_t, fabsf(x));

    if (i == 0 || i >= 0x7F800000) {
        *exp = 0;
        return x;
    }

    i = normalizef_(i);
    *exp = (i >> 23) - 126;

    return copysignf(reinterpret(float, (i & 0x007FFFFF) | 0x3F000000), x);
}

#include "exp2f.h"
#include "finite/log2f.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static float unsigned_(float x, float y)
{
    if (x == 1)
        return 1;

    if (x == 0)
        return signbit(y) ? INFINITY : 0;

    if (isinf(x))
        return signbit(y) ? 0 : INFINITY;

    if (signbit(x))
        return NAN;

    return exp2f_(y * log2f_(x));
}

float powf(float x, float y)
{
    uint32_t sign = 0;

    if (y == 0)
        return 1;

    if (signbit(x) && rintf(y) == y) {
        x = -x;
        sign = -(rintf(y / 2) != y / 2);
    }

    return copysignf(unsigned_(x, y), reinterpret(float, sign));
}

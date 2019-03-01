#include "../reinterpret.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

float nextafterf(float from, float to)
{
    if (from == to || to != to)
        return to;

    if (from != from)
        return from;

    if (from == 0)
        return copysignf(FLT_TRUE_MIN, to);

    int32_t a = reinterpret(int32_t, from);
    int32_t b = reinterpret(int32_t, to);

    return reinterpret(float, b < a || (a ^ b) < 0 ? a - 1 : a + 1);
}

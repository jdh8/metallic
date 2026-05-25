#include "../reinterpret.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

/* See src/math/double/nexttoward.c for the design rationale. */
float nexttowardf(float from, long double to)
{
    if (to != to)
        return (float)to;

    if (from != from)
        return from;

    long double lfrom = (long double)from;

    if (lfrom == to)
        return (float)to;

    if (from == 0)
        return __builtin_copysignf(FLT_TRUE_MIN, (float)to);

    int32_t a = reinterpret(int32_t, from);

    _Bool toward_zero = (from > 0 && to < lfrom) || (from < 0 && to > lfrom);
    return reinterpret(float, toward_zero ? a - 1 : a + 1);
}

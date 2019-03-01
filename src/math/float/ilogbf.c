#include "../reinterpret.h"
#include <math.h>
#include <limits.h>
#include <stdint.h>

int ilogbf(float x)
{
    int32_t magnitude = reinterpret(int32_t, fabsf(x));

    if (magnitude == 0)
        return FP_ILOGB0;

    if (magnitude < 0x00800000)
        return -(118 + __builtin_clz(magnitude));

    if (magnitude < 0x7F800000)
        return (magnitude >> 23) - 127;

    if (FP_ILOGBNAN != INT_MAX && magnitude == 0x7F800000)
        return INT_MAX;

    return FP_ILOGBNAN;
}

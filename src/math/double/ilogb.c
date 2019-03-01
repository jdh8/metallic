#include "../reinterpret.h"
#include <math.h>
#include <limits.h>
#include <stdint.h>

int ilogb(double x)
{
    int64_t word = reinterpret(int64_t, fabs(x));

    if (word == 0)
        return FP_ILOGB0;

    if (word < 0x0010000000000000)
        return -(1011 + __builtin_clzll(word));

    if (word < 0x7FF0000000000000)
        return (word >> 52) - 1023;

    if (FP_ILOGBNAN != INT_MAX && word == 0x7FF0000000000000)
        return INT_MAX;

    return FP_ILOGBNAN;
}

#include "../reinterpret.h"
#include <float.h>
#include <stdint.h>

static double coeff_(Integer exp)
{
    if (exp >= 1024)
        return DBL_MAX;

    if (exp < -1022)
        return 0x1p-1023;

    return reinterpret(double, (uint64_t)(1023 + exp) << 52);
}

float SCALBNF(float x, Integer exp)
{
    return x * coeff_(exp);
}

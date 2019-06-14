#include "finite/log1pf.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

float log1pf(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i == 0xBF800000)
        return -INFINITY;

    if (i > 0xBF800000)
        return NAN;

    if (i < 0x7F800000)
        return log1pf_(x);

    return x;
}

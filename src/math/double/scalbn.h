#include "normalize.h"
#include "../reinterpret.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

double SCALBN(double x, Integer exp)
{
    int64_t i = reinterpret(int64_t, fabs(x));

    if (i == 0 || i >= 0x7FF0000000000000)
        return x;

    i = _normalize(i);

    int64_t biased = exp + (i >> 52);

    if (biased >= 0x7FF || biased < -53)
        return x * (exp < 0 ? 0 : HUGE_VAL);

    i &= 0x000FFFFFFFFFFFFF;

    if (biased > 0)
        return copysign(reinterpret(double, i | biased << 52), x);

    return copysign(0x1p-53, x) * reinterpret(double, i | (biased + 53) << 52);
}

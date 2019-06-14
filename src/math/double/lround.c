#include "round.h"
#include <math.h>
#include <limits.h>

long lround(double x)
{
    double y = LONG_MAX < 0x1p52 ? x + copysign(0.5, x) : round_(x);

    return y >= LONG_MAX ? LONG_MAX : y > LONG_MIN ? (long)y : LONG_MIN;
}

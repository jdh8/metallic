#include "finite/log1pf.h"
#include "atan2f.h"
#include <complex.h>

static float _Complex finite_catanf_(double x, double y)
{
    double xx = x * x;
    double yy = y * y;
    double t = y - 1;
    float a = atan2f_(2 * x, 1 - xx - yy);
    float b = log1pf_(4 * y / (xx + t * t));

    return CMPLXF(0.5f * a, 0.25f * b);
}

static float _Complex catanf_(float x, float y)
{
    const float pi_2 = 1.57079632679489661923;

    if (x == INFINITY)
        return pi_2;

    if (y == INFINITY)
        return x == x ? pi_2 : x;

    if (x == 0 && y == 1)
        return CMPLXF(0, INFINITY);

    if (y != y)
        return CMPLXF(y, y);

    if (x != x)
        return CMPLXF(x, y ? x : y);

    return finite_catanf_(x, y);
}

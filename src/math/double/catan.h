#include <complex.h>
#include <math.h>

static double _Complex finite_catan_(double x, double y)
{
    double xx = x * x;
    double yy = y * y;
    double t = y - 1;
    double a = atan2(2 * x, 1 - xx - yy);
    double b = log1p(4 * y / (xx + t * t));

    return CMPLX(0.5 * a, 0.25 * b);
}

static double _Complex catan_(double x, double y)
{
    const double pi_2 = 1.57079632679489661923;

    if (x == INFINITY)
        return pi_2;

    if (y == INFINITY)
        return x == x ? pi_2 : x;

    if (x == 0 && y == 1)
        return CMPLX(0, INFINITY);

    if (y != y)
        return CMPLX(y, y);

    if (x != x)
        return CMPLX(x, y ? x : y);

    return finite_catan_(x, y);
}

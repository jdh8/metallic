#include "finite/cash.h"

static double _Complex casinh_(double x, double y)
{
    const double pi = 3.14159265358979323846;

    if (x == INFINITY)
        return CMPLX(INFINITY, y == INFINITY ? pi / 4 : 0 * y);

    if (y == INFINITY)
        return CMPLX(INFINITY, x == x ? pi / 2 : x);

    if (isunordered(x, y))
        return CMPLX(NAN, y ? NAN : 0);

    return cash_(x, y, carg_);
}

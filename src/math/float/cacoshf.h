#include "finite/cashf.h"

static double _atan2(double y, double x)
{
    return _carg(x, y);
}

static double _Complex _cacoshf(float x, float y)
{
    const double pi = 3.14159265358979323846;

    if (x == INFINITY)
        return CMPLX(INFINITY, y == INFINITY ? pi / 4 : 0 * y);

    if (y == INFINITY)
        return CMPLX(INFINITY, x == x ? pi / 2 : x);

    if (isunordered(x, y))
        return CMPLX(NAN, x ? NAN : pi / 2);

    return _cashf(y, x, _atan2);
}

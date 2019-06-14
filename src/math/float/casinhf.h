#include "finite/cashf.h"

static float _Complex casinhf_(float x, float y)
{
    const double pi = 3.14159265358979323846;

    if (x == INFINITY)
        return CMPLXF(INFINITY, y == INFINITY ? pi / 4 : 0 * y);

    if (y == INFINITY)
        return CMPLXF(INFINITY, x == x ? pi / 2 : x);

    if (isunordered(x, y))
        return CMPLXF(NAN, y ? NAN : 0);

    return cashf_(x, y, carg_);
}

#include "cisf.h"
#include "expm1f.h"
#include <complex.h>

static float _Complex _csinhf(float x, float y)
{
    double t = _expm1f(fabsf(x));
    double cosh = 0.5 * (t + 1) + 0.5 / (t + 1);
    double sinh = copysign(t, x) * (0.5 + 0.5 / (t + 1));
    double _Complex circular = _cisf(y);

    if (y == 0)
        return CMPLXF(sinh, y);

    if (y - y && (x == 0 || isinf(x)))
        return CMPLXF(x, y - y);

    return CMPLXF(sinh * creal(circular), cosh * cimag(circular));
}

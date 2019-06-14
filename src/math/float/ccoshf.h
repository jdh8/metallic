#include "cisf.h"
#include "expm1f.h"
#include <complex.h>

static float _Complex ccoshf_(float x, float y)
{
    double t = expm1f_(fabsf(x));
    double cosh = 0.5 * (t + 1) + 0.5 / (t + 1);
    double sinh = copysign(t, x) * (0.5 + 0.5 / (t + 1));
    double _Complex circular = cisf_(y);

    if (y == 0) {
        uint32_t i = reinterpret(uint32_t, copysignf(0, x));
        uint32_t j = reinterpret(uint32_t, y);
        return CMPLXF(cosh, reinterpret(float, i ^ j));
    }

    if (y - y) {
        if (x == 0)
            return CMPLXF(y - y, x);

        if (isinf(x))
            return CMPLXF(INFINITY, y - y);
    }

    return CMPLXF(cosh * creal(circular), sinh * cimag(circular));
}

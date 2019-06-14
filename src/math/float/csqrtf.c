#include "finite/csqrt.h"
#include <complex.h>
#include <math.h>

float _Complex csqrtf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    if (z == 0)
        return CMPLXF(0, y);

    if (isinf(y))
        return CMPLXF(INFINITY, y);

    if (x == INFINITY)
        return CMPLXF(x, 0 * y);

    if (x == -INFINITY)
        return CMPLXF(y - y, copysignf(x, y));

    return csqrt_(x, y);
}

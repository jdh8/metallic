#include "cacoshf.h"

float _Complex cacosf(float _Complex z)
{
    const double pi = 3.14159265358979323846;

    float x = z;
    float y = cimagf(z);

    double _Complex first = _cacoshf(fabsf(x), fabsf(y));

    return CMPLXF(fabs(cimag(first) - (x < 0) * pi), copysignf(first, -y));
}

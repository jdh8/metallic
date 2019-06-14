#include "cacoshf.h"

float _Complex cacoshf(float _Complex z)
{
    const double pi = 3.14159265358979323846;

    float x = z;
    float y = cimagf(z);

    double _Complex first = cacoshf_(fabsf(x), fabsf(y));

    return CMPLXF(first, copysignf(cimag(first) - (x < 0) * pi, y));
}

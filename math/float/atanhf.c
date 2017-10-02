#include "atanhf/taylor.h"
#include "quietf.h"
#include <math.h>

static double _finite(double x)
{
    const double ln2_2 = 0.3465735902799726547;

    int exponent;
    double y = logf_reduce((1 + x) / (1 - x), &exponent);

    if (exponent)
        return atanhf_taylor((y - 1) / (y + 1)) + exponent * ln2_2;
    else
        return atanhf_taylor(x);
}

float atanhf(float x)
{
    float absx = fabsf(x);

    if (absx == 1)
        return copysignf(HUGE_VALF, x);

    if (absx < 1)
        return _finite(x);

    return quietf(x);
}

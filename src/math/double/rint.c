#include "../rounding.h"
#include <math.h>

static double rint_(double x)
{
    const double rectifier = 0x1p52;
    double r = fabs(x);

    if (r < rectifier) {
        double y = r + rectifier;
        return copysign(y - rectifier, x);
    }

    return x;
}

double rint(double x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_rint(x);
#endif
    return rint_(x);
}

#ifdef __wasm__
double nearbyint(double) __attribute__((alias("rint")));
#endif

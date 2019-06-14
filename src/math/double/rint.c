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

#if defined(__wasm__) || defined(__AVX__) || defined(__SSE4_1__)
#define RINT(x) __builtin_rint(x)
#else
#define RINT(x) rint_(x)
#endif

double rint(double x)
{
    (void)rint_;
    return RINT(x);
}

#ifdef __wasm__
double nearbyint(double) __attribute__((alias("rint")));
#endif

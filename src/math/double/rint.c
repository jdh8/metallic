#include "rint.h"
#include "../rounding.h"

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

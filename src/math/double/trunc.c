#include "trunc.h"
#include "../rounding.h"

double trunc(double x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_trunc(x);
#endif
    return trunc_(x);
}

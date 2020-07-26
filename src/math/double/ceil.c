#include "ceil.h"
#include "../rounding.h"

double ceil(double x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_ceil(x);
#endif
    return ceil_(x);
}

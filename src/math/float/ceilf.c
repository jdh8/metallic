#include "ceilf.h"
#include "../rounding.h"

float ceilf(float x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_ceilf(x);
#endif
    return ceilf_(x);
}

#include "truncf.h"
#include "../rounding.h"

float truncf(float x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_truncf(x);
#endif
    return truncf_(x);
}

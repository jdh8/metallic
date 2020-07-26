#include "floor.h"
#include "../rounding.h"

double floor(double x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_floor(x);
#endif
    return floor_(x);
}

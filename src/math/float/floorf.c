#include "floorf.h"
#include "../rounding.h"

float floorf(float x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_floorf(x);
#endif
    return floorf_(x);
}

#include "rintf.h"
#include "../rounding.h"

float rintf(float x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_rintf(x);
#endif
    return rintf_(x);
}

#ifdef __wasm__
float nearbyintf(float) __attribute__((alias("rintf")));
#endif

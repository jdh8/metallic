#include "../rounding.h"
#include <math.h>

static float rintf_(float x)
{
    const float rectifier = 0x1p23f;
    float r = fabsf(x);

    if (r < rectifier) {
        float y = r + rectifier;
        return copysignf(y - rectifier, x);
    }

    return x;
}

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

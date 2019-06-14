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

#if defined(__wasm__) || defined(__AVX__) || defined(__SSE4_1__)
#define RINTF(x) __builtin_rintf(x)
#else
#define RINTF(x) rintf_(x)
#endif

float rintf(float x)
{
    (void)rintf_;
    return RINTF(x);
}

#ifdef __wasm__
float nearbyintf(float) __attribute__((alias("rintf")));
#endif

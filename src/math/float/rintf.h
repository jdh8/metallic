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

#include "../reinterpret.h"
#include "../rounding.h"
#include <math.h>
#include <stdint.h>

static float ceilf_(float x)
{
    int32_t bits = reinterpret(int32_t, x);
    int32_t magnitude = bits & 0x7FFFFFFF;

    if (magnitude == 0 || magnitude >= 0x4B000000) /* 0x1p23 */
        return x;

    if (magnitude < 0x3F800000) /* 1 */
        return -(float)~(bits >> 31);

    int32_t mask = 0x007FFFFF >> ((magnitude >> 23) - 127);

    return reinterpret(float, (bits + (bits >= 0) * mask) & ~mask);
}

float ceilf(float x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_ceilf(x);
#endif
    return ceilf_(x);
}

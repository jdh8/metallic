#include "../reinterpret.h"
#include "../rounding.h"
#include <stdint.h>

static float truncf_(float x)
{
    const int32_t mask = 0xFF800000;
    int32_t bits = reinterpret(int32_t, x);
    int32_t magnitude = bits & 0x7FFFFFFF;

    if (magnitude < 0x3F800000) /* 1 */
        return reinterpret(float, bits & 0x80000000);

    if (magnitude < 0x4B000000) /* 0x1p23 */
        return reinterpret(float, mask >> ((magnitude >> 23) - 127) & bits);

    return x;
}

float truncf(float x)
{
#ifdef METALLIC_FAST_ROUNDING
    return __builtin_truncf(x);
#endif
    return truncf_(x);
}

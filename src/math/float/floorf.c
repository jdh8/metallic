#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static float _floorf(float x)
{
    int32_t bits = reinterpret(int32_t, x);
    int32_t magnitude = bits & 0x7FFFFFFF;

    if (magnitude == 0 || magnitude >= 0x4B000000) /* 0x1p23 */
        return x;

    if (magnitude < 0x3F800000) /* 1 */
        return bits >> 31;

    int32_t mask = 0x007FFFFF >> ((magnitude >> 23) - 127);

    return reinterpret(float, (bits + (bits < 0) * mask) & ~mask);
}

#if defined(__wasm__) || defined(__AVX__) || defined(__SSE4_1__)
#define FLOORF(x) __builtin_floorf(x)
#else
#define FLOORF(x) _floorf(x)
#endif

float floorf(float x)
{
    (void)_floorf;
    return FLOORF(x);
}

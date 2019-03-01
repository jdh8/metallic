#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _floor(double x)
{
    int64_t bits = reinterpret(int64_t, x);
    int64_t magnitude = bits & 0x7FFFFFFFFFFFFFFF;

    if (magnitude == 0 || magnitude >= 0x4330000000000000) /* 0x1p52 */
        return x;

    if (magnitude < 0x3FF0000000000000) /* 1 */
        return bits >> 63;

    int64_t mask = 0x000FFFFFFFFFFFFF >> ((magnitude >> 52) - 1023);

    return reinterpret(double, (bits + (bits < 0) * mask) & ~mask);
}

#if defined(__wasm__) || defined(__AVX__) || defined(__SSE4_1__)
#define FLOOR(x) __builtin_floor(x)
#else
#define FLOOR(x) _floor(x)
#endif

double floor(double x)
{
    (void)_floor;
    return FLOOR(x);
}

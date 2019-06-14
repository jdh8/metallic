#include "../reinterpret.h"
#include <stdint.h>

static double trunc_(double x)
{
    const int64_t mask = 0xFFF0000000000000;
    int64_t bits = reinterpret(int64_t, x);
    int64_t magnitude = bits & 0x7FFFFFFFFFFFFFFF;

    if (magnitude < 0x3FF0000000000000) /* 1 */
        return reinterpret(double, bits & 0x8000000000000000);

    if (magnitude < 0x4330000000000000) /* 0x1p52 */
        return reinterpret(double, mask >> ((magnitude >> 52) - 1023) & bits);

    return x;
}

#if defined(__wasm__) || defined(__AVX__) || defined(__SSE4_1__)
#define TRUNC(x) __builtin_trunc(x)
#else
#define TRUNC(x) trunc_(x)
#endif

double trunc(double x)
{
    (void)trunc_;
    return TRUNC(x);
}

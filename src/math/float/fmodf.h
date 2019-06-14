#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static uint32_t significand_(uint32_t i)
{
    return (i & 0x007FFFFF) | 0x00800000;
}

static uint32_t remshift_(uint32_t a, uint32_t b, uint32_t exp)
{
    uint_fast64_t remainder = ((uint_fast64_t)a << (exp & 31)) % b;

    for (uint32_t shift = exp >> 5; shift; --shift)
        remainder = (remainder << 32) % b;

    return remainder;
}

static int32_t load_(int32_t remainder, int32_t template)
{
    if (remainder == 0)
        return remainder;

    int32_t shift = __builtin_clz(remainder) - 8;
    int32_t exp = (template >> 23) - shift;
    int32_t significand = remainder << shift;
    int32_t normalized = exp << 23 | (significand & 0x007FFFFF);

    return normalized < 0x00800000 ? significand >> (1 - exp) : normalized;
}

static uint32_t finite_(uint32_t a, uint32_t b)
{
    if (b <= 0x01000000)
        return a <= 0x01000000 ? a % b : remshift_(significand_(a), b, (a >> 23) - 1);

    return load_(remshift_(significand_(a), significand_(b), (a >> 23) - (b >> 23)), b);
}

static float fmodf_(float numerator, float denominator)
{
    uint32_t a = reinterpret(uint32_t, fabsf(numerator));
    uint32_t b = reinterpret(uint32_t, fabsf(denominator));

    if (a >= 0x7F800000 || b > 0x7F800000 || b == 0)
        return NAN;

    if (a < b)
        return numerator;

    return copysignf(reinterpret(float, finite_(a, b)), numerator);
}

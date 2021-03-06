#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static uint64_t significand_(uint64_t i)
{
    const uint64_t min = 0x0010000000000000;

    return (i & (min - 1)) | min;
}

static uint64_t min_(uint64_t a, uint64_t b)
{
    return a < b ? a : b;
}

static uint64_t remshift_(uint64_t a, uint64_t b, uint64_t exp)
{
    uint64_t shift = min_(__builtin_ctzll(b), exp);
    uint64_t denominator = b >> shift;
    uint64_t remainder = a % denominator;
    uint64_t space = __builtin_clzll(denominator);

    for (exp -= shift; exp >= space; exp -= space)
        remainder = (remainder << space) % denominator;

    return ((remainder << exp) % denominator) << shift;
}

static int64_t load_(int64_t remainder, int64_t template)
{
    const int64_t min = 0x0010000000000000;

    if (remainder == 0)
        return remainder;

    int64_t shift = __builtin_clzll(remainder) - 11;
    int64_t exp = (template >> 52) - shift;
    int64_t significand = remainder << shift;
    int64_t normalized = exp << 52 | (significand & (min - 1));

    return normalized < min ? significand >> (1 - exp) : normalized;
}

static uint64_t finite_(uint64_t a, uint64_t b)
{
    const uint64_t threshold = 0x0020000000000000;

    if (b <= threshold)
        return a <= threshold ? a % b : remshift_(significand_(a), b, (a >> 52) - 1);

    return load_(remshift_(significand_(a), significand_(b), (a >> 52) - (b >> 52)), b);

}

static double fmod_(double numerator, double denominator)
{
    const uint64_t inf = 0x7FF0000000000000;

    uint64_t a = reinterpret(uint64_t, fabs(numerator));
    uint64_t b = reinterpret(uint64_t, fabs(denominator));

    if (a >= inf || b > inf || b == 0)
        return NAN;

    if (a < b)
        return numerator;

    return copysign(reinterpret(double, finite_(a, b)), numerator);
}

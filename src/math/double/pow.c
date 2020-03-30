#include "kernel/log.h"
#include "normalize.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Truncate the least significant bits */
static double trunc_(double x, int bits)
{
    uint64_t i = reinterpret(uint64_t, x) & (uint64_t)-1 << bits;
    return reinterpret(double, i);
}

/* Compute log2 of normalized representation
 *
 * i    - Normalized bits of x
 * y[0] - Most significant 32 bits of log2(x), truncated
 * y[1] - Approximate log2(x) - y[0]
 */
static void log2_(double y[static 2], int64_t i)
{
    const double log2e[] = { 0x1.71547652p0, 0x1.705fc2eefa2p-33 };

    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double x = reinterpret(double, i - (exponent << 52)) - 1;
    double z = x / (x + 2);
    double h = 0.5 * x * x;
    double a = trunc_(x - h, 32);
    double b = x - a - h + z * (h + kernel_log_(z));
    double u = a * log2e[0];
    double v = (a + b) * log2e[1] + b * log2e[0];

    y[0] = trunc_(u + v + exponent, 21);
    y[1] = exponent - y[0] + u + v;
}

/* Comppute 2^(a + b) where a ≥ b ≥ 0 or a ≤ b ≤ 0 */
static double exp2_(double a, double b)
{
    double s = a + b;

    if (s > 1024 || (s == 1024 && b > s - a))
        return HUGE_VAL;

    if (s < -1075 || (s == -1075 && b < s - a))
        return 0;

    double e = b - (s - a);

    //TODO
}

static double unsigned_(double x, double y)
{
    if (x == 1)
        return 1;

    if (x == 0)
        return signbit(y) ? HUGE_VAL : 0;

    if (isinf(x))
        return signbit(y) ? 0 : HUGE_VAL;

    if (signbit(x))
        return NAN;

    double y0 = trunc_(y, 32);
    double t[2];

    log2_(t, normalize_(reinterpret(int64_t, x)));

    return exp2_(y0 * t[0], (y - y0) * t[0] + y * t[1]);
}

double pow(double x, double y)
{
    uint64_t sign = 0;

    if (y == 0)
        return 1;

    if (signbit(x) && rintf(y) == y) {
        x = -x;
        sign = (uint64_t)(rintf(y / 2) != y / 2) << 63;
    }

    uint64_t magnitude = reinterpret(uint64_t, unsigned_(x, y));
    return reinterpret(double, magnitude | sign);
}


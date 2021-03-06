#include "kernel/exp.h"
#include "normalize.h"
#include "truncate.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Restriction of (x -> 3 atanh(x) / x - 3 - x^2) to [-c, c], where
 *
 *     √2 - 1                  1 + c
 * c = ------  the solution to ----- = √2.
 *     √2 + 1,                 1 - c
 */
static double log_kernel_(double x)
{
    const double c[] = {
        0.60000000000000910393,
        0.42857142856356307074,
        0.33333333563056661945,
        0.27272695496468461223,
        0.23079269122036327328,
        0.19905203560197025739,
        0.19611466759635016238
    };

    x *= x;

    return x * x * ((((((c[6] * x + c[5]) * x + c[4]) * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0]);
}

/* Compute log2 of normalized representation
 *
 * This function returns truncated most significant 32 bits of log2(x).
 * Approximate residual log2(x) is written to *residue.
 *
 * i - Normalized bits of x
 */
static double log2_(int64_t i, double residue[static 1])
{
    const double log8e2[] = { 0x1.ec709dc4p-1, -0x1.7f00a2d80faabp-35 };

    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double x = reinterpret(double, i - (exponent << 52)) - 1;
    double w = truncate_(x + 2, 32);
    double z = x / (x + 2);
    double z0 = truncate_(z, 27);
    double z1 = (x - z0 * w - z0 * (2 - w + x)) / (x + 2);
    double h = z0 * z0;
    double r = log_kernel_(z) + z1 * (z0 + z);
    double t = truncate_(h + r + 3, 26);
    double a = z0 * t;
    double b = z1 * t + z * (3 - t + h + r);
    double s = truncate_(a + b, 32);
    double u = s * log8e2[0];
    double v = s * log8e2[1] + (a - s + b) * (log8e2[0] + log8e2[1]);
    double y = truncate_(u + v + exponent, 21);

    *residue = exponent - y + u + v;
    return y;
}

/* Comppute 2^(a + b) where a ≥ b ≥ 0 or a ≤ b ≤ 0 */
static double exp2_(double a, double b)
{
    const double ln2[] = { 0x1.62e42ffp-1, -0x1.718432a1b0e26p-35 };

    double s = a + b;

    if (s > 1024 || (s == 1024 && b > s - a))
        return HUGE_VAL;

    if (s < -1075 || (s == -1075 && b < s - a))
        return 0;

    double n = rint(s);
    double t = s - n;
    double t0 = truncate_(t, 32);
    double u = t0 * ln2[0];
    double v = t * ln2[1] + (a - (n + t0) + b) * ln2[0];
    int64_t i = reinterpret(int64_t, kernel_expb_(u, v) + 1) + ((int64_t)n << 52);

    if (s < -1020)
        return 0x1p-1020 * reinterpret(double, i + 0x3FC0000000000000);

    return reinterpret(double, i);
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

    if (isinf(y))
        return signbit(y) ^ (x < 1) ? 0 : HUGE_VAL;

    double t1;
    double t0 = log2_(normalize_(reinterpret(int64_t, x)), &t1);
    double y0 = truncate_(y, 32);

    return exp2_(y0 * t0, (y - y0) * t0 + y * t1);
}

double pow(double x, double y)
{
    uint64_t sign = 0;

    if (y == 0)
        return 1;

    if (signbit(x) && rint(y) == y) {
        x = -x;
        sign = (uint64_t)(rint(y / 2) != y / 2) << 63;
    }

    uint64_t magnitude = reinterpret(uint64_t, unsigned_(x, y));
    return reinterpret(double, magnitude | sign);
}

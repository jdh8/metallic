/* C23 exp10m1f: 10^x - 1, correctly rounded.
 *
 * Port of metallic-rs `exp10m1f` (src/f32_/exp.rs): exp2m1f's structure over
 * the base-10 grid a = 32*log2(10)*x (the residual's rounding stays below
 * 2^-46 of the result), with its own Taylor small band and the same shared
 * table and kernel.  One sweep-pinned tie precedes the double fallback.
 * Certified by the exhaustive 2^32 sweep in the oracle suite. */

#include "../reinterpret.h"
#include "../double/shift.h"
#include "kernel/exp32f.h"
#include <math.h>
#include <stdint.h>

double exp10m1(double);

/* (10^x - 1)/x Taylor slope ln10^(k+1)/(k+1)! for the small band. */
static const double exp10m1f_slope_[6] = {
    0x1.26bb1bbb55516p+1, 0x1.53524c73cea69p+1, 0x1.0470591de2ca4p+1,
    0x1.2bd7609fd98c4p+0, 0x1.1429ffd1d4d76p-1, 0x1.a7ed70847c8b6p-3,
};

float exp10m1f(float x)
{
    if (x <= -8.0f)
        return -1.0f; /* 10^x <= 10^-8 << 2^-25 (-inf included) */

    if (x >= 39.0f)
        return INFINITY; /* 10^39 > 2^128 (+inf included); NaN falls through */

    const double big = 0x1.8p52;
    double xd = x;
    double a = xd * 0x1.a934f0979a371p+6; /* 32 log2(10) */
    double abig = a + big;
    double m = abig - big;

    if (m == 0.0) {
        double p = ((((exp10m1f_slope_[5] * xd + exp10m1f_slope_[4]) * xd
                  + exp10m1f_slope_[3]) * xd + exp10m1f_slope_[2]) * xd
                  + exp10m1f_slope_[1]) * xd + exp10m1f_slope_[0];
        return xd * p;
    }

    uint64_t u = reinterpret(uint64_t, abig);
    int64_t q = ((int64_t)(u & 0x000FFFFFFFFFFFFF) - 0x0008000000000000) >> 5;
    double h = a - m;
    double sv = shift_(exp32f_t_[u & 31], q);
    double p = (((exp32f_p_[4] * h + exp32f_p_[3]) * h + exp32f_p_[2]) * h
             + exp32f_p_[1]) * h + exp32f_p_[0];
    double r = p * sv - 1.0;

    double eps = sv * 0x1p-42;
    float lower = (float)(r - eps);

    if (lower == (float)(r + eps))
        return lower;

    /* The one f32 tie the fallback's double rounding cannot steer. */
    if (reinterpret(uint64_t, xd) == 0x402FAFEC00000000)
        return reinterpret(float, (uint32_t)0x59C64405);

    return exp10m1(xd);
}

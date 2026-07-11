/* C23 exp2m1f: 2^x - 1, correctly rounded.
 *
 * Port of metallic-rs `exp2m1f` (src/f32_/exp.rs): expm1f's 1/32-step
 * reduction with an *exact* base-2 residual (a = 32x and h = a - m are
 * exact), the shared 2^(j/32) table and 2^(h/32) kernel, and an own-Taylor
 * small band.  A straddle of the +-sv*2^-42 gate falls back to the
 * correctly rounded double exp2m1.  Certified by the exhaustive 2^32 sweep
 * in the oracle suite. */

#include "../reinterpret.h"
#include "../double/shift.h"
#include "kernel/exp32f.h"
#include <math.h>
#include <stdint.h>

double exp2m1(double);

/* (2^x - 1)/x Taylor slope ln2^(k+1)/(k+1)! for the |x| < 1/64 band;
 * the degree-5 tail sits near 2^-49 relative. */
static const double exp2m1f_slope_[6] = {
    0x1.62e42fefa39efp-1, 0x1.ebfbdff82c58ep-3, 0x1.c6b08d704a0c0p-5,
    0x1.3b2ab6fba4e77p-7, 0x1.5d87fe78a6731p-10, 0x1.430912f86c787p-13,
};

float exp2m1f(float x)
{
    if (x <= -25.0f)
        return -1.0f; /* 2^x <= 2^-25: within half an ulp of -1 (-inf too) */

    if (x >= 128.0f)
        return INFINITY; /* overflow (+inf included); NaN falls through */

    const double big = 0x1.8p52;
    double xd = x;
    double a = 32.0 * xd;
    double abig = a + big;
    double m = abig - big;

    if (m == 0.0) {
        double p = ((((exp2m1f_slope_[5] * xd + exp2m1f_slope_[4]) * xd
                  + exp2m1f_slope_[3]) * xd + exp2m1f_slope_[2]) * xd
                  + exp2m1f_slope_[1]) * xd + exp2m1f_slope_[0];
        return xd * p;
    }

    uint64_t u = reinterpret(uint64_t, abig);
    int64_t q = ((int64_t)(u & 0x000FFFFFFFFFFFFF) - 0x0008000000000000) >> 5;
    double h = a - m; /* exact */
    double sv = shift_(exp32f_t_[u & 31], q);
    double p = (((exp32f_p_[4] * h + exp32f_p_[3]) * h + exp32f_p_[2]) * h
             + exp32f_p_[1]) * h + exp32f_p_[0];
    double r = p * sv - 1.0;

    double eps = sv * 0x1p-42;
    float lower = (float)(r - eps);

    if (lower == (float)(r + eps))
        return lower;

    return exp2m1(xd);
}

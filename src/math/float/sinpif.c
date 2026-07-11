/* C23 sinpif: sin(pi x), correctly rounded.
 *
 * Port of metallic-rs `sinpif` (src/f32_/trig.rs) -- CORE-MATH cr_sinpif's
 * structure with metallic-rs's own Taylor residuals: an integer reduction of
 * the signed mantissa yields the 1/64-half-turn grid index and the exact
 * fixed-point residual, one 128-entry signed table covers the full circle
 * (cosine by +32 offset), and plain-f64 residual polynomials finish with a
 * single rounding -- no Ziv gate needed at f32 precision.  Certified by the
 * exhaustive 2^32 sweep in the oracle suite. */

#include "../reinterpret.h"
#include "kernel/sinpif.h"
#include <math.h>
#include <stdint.h>

float sinpif(float x)
{
    uint32_t ix = reinterpret(uint32_t, x);
    int32_t e = (ix >> 23) & 0xFF;

    if (e == 0xFF)
        return (x - x) / (x - x); /* +-inf and NaN */

    int32_t sgn = (int32_t)ix >> 31;
    int32_t m = ((int32_t)((ix & (~0u >> 9)) | 1 << 23) ^ sgn) - sgn;
    int32_t s = 143 - e;

    if (s < 0) {
        /* |x| >= 2^17: every value sits exactly on the 1/64 grid. */
        if (s < -6)
            return copysignf(0.0f, x); /* |x| >= 2^23: all integers */

        uint32_t iq = ((uint32_t)m << (-s - 1)) & 127;

        if ((iq & 63) == 0)
            return copysignf(0.0f, x); /* integer x */

        return sinpif_s_[iq];
    }

    if (s > 30) {
        /* |x| < 2^-14: sin(pi x) = pi x - (pi x)^3/6 in f64, one rounding out. */
        double z = x;
        return z * (z * z * -0x1.4abbce625be53p+2 + 0x1.921fb54442d18p+1);
    }

    int32_t si = 25 - s;

    if (si >= 0 && (uint32_t)m << si == 0)
        return copysignf(0.0f, x); /* integer x */

    int32_t k = (int32_t)((uint32_t)m << (31 - s));
    double z = k;
    double z2 = z * z;
    double fs = z2 * (z2 * (z2 * sinpif_sn_[3] + sinpif_sn_[2]) + sinpif_sn_[1]) + sinpif_sn_[0];
    double fc = z2 * (z2 * sinpif_cn_[2] + sinpif_cn_[1]) + sinpif_cn_[0];
    int32_t iq = ((m >> s) + 1) >> 1;
    double ts = sinpif_s_[iq & 127];
    double tc = sinpif_s_[(iq + 32) & 127];

    return ts + (ts * z2) * fc + (tc * z) * fs;
}

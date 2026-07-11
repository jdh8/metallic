/* C23 cospif: cos(pi x), correctly rounded.
 *
 * Port of metallic-rs `cospif` (src/f32_/trig.rs) -- CORE-MATH cr_cospif's
 * structure, sharing sinpif's tables: the unsigned mantissa (cosine is even)
 * reduces to the same 1/64-half-turn grid with a +32 quarter-turn shift, and
 * grid-exact inputs read the table directly.  Certified by the exhaustive
 * 2^32 sweep in the oracle suite. */

#include "../reinterpret.h"
#include "kernel/sinpif.h"
#include <math.h>
#include <stdint.h>

float cospif(float x)
{
    uint32_t ix = reinterpret(uint32_t, x);
    int32_t e = (ix >> 23) & 0xFF;

    if (e == 0xFF)
        return (x - x) / (x - x); /* +-inf and NaN */

    uint32_t m = (ix & (~0u >> 9)) | 1 << 23;
    int32_t s = 143 - e;
    int32_t p = e - 112;

    /* |x| < 2^-15: 1 - cos(pi x) < 2^-27.7, under half the ulp gap below 1,
     * so round-to-nearest returns exactly 1 (the reference keeps an FMA here
     * only for directed rounding modes, which WASM does not have). */
    if (p < 0)
        return 1.0f;

    if (p > 31) {
        if (p > 63)
            return 1.0f; /* |x| >= 2^47: all even integers at f32 precision */

        uint32_t iq = m << (p - 32);
        return sinpif_s_[(iq + 32) & 127];
    }

    int32_t k = (int32_t)(m << p);

    if (k == 0) {
        /* Exactly on the 1/64 grid (integers and half-integers included). */
        uint32_t iq = m >> (32 - p);
        return sinpif_s_[(iq + 32) & 127];
    }

    double z = k;
    double z2 = z * z;
    double fs = z2 * (z2 * (z2 * sinpif_sn_[3] + sinpif_sn_[2]) + sinpif_sn_[1]) + sinpif_sn_[0];
    double fc = z2 * (z2 * sinpif_cn_[2] + sinpif_cn_[1]) + sinpif_cn_[0];
    uint32_t iq = ((m >> s) + 1) >> 1;
    double ts = sinpif_s_[(iq + 32) & 127]; /* cosine by quarter-turn shift */
    double tc = sinpif_s_[iq & 127];

    return ts + (ts * z2) * fc - (tc * z) * fs;
}

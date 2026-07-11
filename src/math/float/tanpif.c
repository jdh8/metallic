/* C23 tanpif: tan(pi x), correctly rounded.
 *
 * Port of metallic-rs `tanpif` (src/f32_/trig.rs) -- CORE-MATH cr_tanpif's
 * structure: one odd rational in f64 over the reduced period
 * z = x - round(x), with the poles and zeros carried by exact factors and
 * quarter-integers returned as +-1 / signed zero / +-inf up front.
 * Certified by the exhaustive 2^32 sweep in the oracle suite. */

#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Odd rational tan(pi z)*(1/4 - z^2)/(z - z^3) numerator/denominator
 * coefficients over the reduced period; the (1/4 - z^2) factor bakes in the
 * poles and (z - z^3) the zeros.  Generated with `ratapprox
 * --function="tan(pi*sqrt(x))*(0.25-x)/(sqrt(x)*(1-x))"
 * --dom="[1e-9,0.24999998]" --type=[3,3] --numF=[D] --denF=[D]`. */
static const double tanpif_cn_[4] = {
    0x1.921fb54442d19p-1, -0x1.1f452e4d0d93fp-2, 0x1.68a1b6f365c62p-6, -0x1.e48093d13a5cep-13,
};
static const double tanpif_cd_[4] = {
    0x1p+0, -0x1.4b4b5da7bba8dp-1, 0x1.8e981cbe04ec4p-4, -0x1.a6f463eb79f76p-9,
};

float tanpif(float x)
{
    uint32_t ix = reinterpret(uint32_t, x);
    uint32_t e = ix & (0xFFu << 23);

    if (e > 150u << 23) {
        if (e == 0xFFu << 23)
            return ix << 9 == 0 ? (x - x) / (x - x) : x + x; /* inf -> NaN */

        return copysignf(0.0f, x); /* |x| > 2^23: all even integers */
    }

    float x4 = 4.0f * x;
    float dx4 = x4 - rintf(x4);
    float zf = x - rintf(x);

    if (dx4 == 0.0f) {
        /* 4x is an integer: the exact +-1 / signed-zero / pole lattice.
         * |4x| <= 2^25 fits an int32_t. */
        int32_t k = (int32_t)x4;

        if (k & 1)
            return copysignf(1.0f, zf); /* x = 1/4 mod 1/2 */

        switch (k & 6) {
        case 0:
            return copysignf(0.0f, x); /* x = 0 mod 2 */
        case 4:
            return -copysignf(0.0f, x); /* x = 1 mod 2 */
        case 2:
            return INFINITY; /* x = 1/2 mod 2 */
        default:
            return -INFINITY; /* x = -1/2 mod 2 */
        }
    }

    double z = zf;
    double z2 = z * z;
    double z4 = z2 * z2;

    return (z - z * z2)
         * ((tanpif_cn_[0] + z2 * tanpif_cn_[1]) + z4 * (tanpif_cn_[2] + z2 * tanpif_cn_[3]))
         / (((tanpif_cd_[0] + z2 * tanpif_cd_[1]) + z4 * (tanpif_cd_[2] + z2 * tanpif_cd_[3]))
            * (0.25 - z2));
}

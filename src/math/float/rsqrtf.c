/* C23 rsqrtf: 1/sqrt(x), correctly rounded.
 *
 * Port of metallic-rs `rsqrtf` (src/f32_/misc.rs): the promoted division and
 * square root leave the double result accurate enough that its single
 * rounding to float is correct for all but one subnormal input and two
 * mantissa families, which CORE-MATH's cr_rsqrtf patches with a constructed
 * r - dr in the result's own binade (ported verbatim, wrapping arithmetic
 * included).  Certified by the exhaustive 2^32 sweep in the oracle suite. */

#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

float rsqrtf(float x)
{
    uint32_t ix = reinterpret(uint32_t, x);

    if (ix >= 0xFFu << 23 || ix == 0) {
        if (ix << 1 == 0)
            return 1.0f / x; /* pole at +-0, sign preserved */

        if (x != x)
            return x + x; /* quiet NaN, payload preserved */

        if (ix == 0xFFu << 23)
            return 0.0f; /* +inf */

        return (x - x) / (x - x); /* x < 0, -inf included: NaN */
    }

    /* The three double-rounding victims of the promoted expression: one
     * subnormal input and two mantissa families across all exponents. */
    uint32_t m = ix << 8;

    if ((ix == 0x002F7E2A || m == 0xBDF8A800 || m == 0x55B7BD00) && ix != 0x0055B7BD) {
        uint32_t e = ix == 0x002F7E2A ? ~0u : ix >> 23;
        e = (512 - e) / 2 - 578;

        uint32_t base = m == 0x55B7BD00 ? 0x000C1740 : 0x005222E0;
        float r = reinterpret(float, base | e << 23);
        float dr = reinterpret(float, (e - 25) << 23);
        return r - dr;
    }

    double xd = x;
    return (1.0 / xd) * sqrt(xd);
}

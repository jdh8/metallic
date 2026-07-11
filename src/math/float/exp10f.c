/* C23 exp10f: 10^x, correctly rounded.
 *
 * Port of metallic-rs `exp10f` (src/f32_/exp.rs): promote to double, one
 * Cody-Waite reduction x - n*log10(2) (the hi word has 15 trailing zero
 * significand bits, so n*hi is exact for |n| < 2^15 and the subtraction is
 * exact by Sterbenz -- no FMA needed), a degree-10 polynomial for 10^r on
 * the reduced band, and one exponent injection.  Single rounding to float;
 * certified by the exhaustive 2^32 sweep in the oracle suite. */

#include "../double/shift.h"
#include <math.h>
#include <stdint.h>

float exp10f(float x)
{
    if (x != x)
        return x;

    /* (MIN_EXP - MANTISSA_DIGITS - 1)*log10(2) and MAX_EXP*log10(2) in
     * float, the same constants the reference compares against. */
    if (x < -0x1.6ba4eep+5f)
        return 0;

    if (x > 0x1.344136p+5f)
        return INFINITY;

    const double c[] = {
        1.0,
        2.3025850929940486,
        2.6509490552392045,
        2.034678592287247,
        1.171255148908203,
        5.393829313950126e-1,
        2.0699584957469658e-1,
        6.808909329404776e-2,
        1.959761565686179e-2,
        5.027633471110143e-3,
        1.1576553790747818e-3
    };

    double xd = x;
    double n = rint(xd * 3.321928094887362); /* log2(10) */
    double r = xd - n * 0.3010299956640665;  /* exact (15 trailing zeros) */
    r -= n * -8.532344317057107e-14;

    double p = c[10];

    for (int k = 9; k >= 0; --k)
        p = p * r + c[k];

    return shift_(p, (int64_t)n);
}

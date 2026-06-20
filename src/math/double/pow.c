#include "kernel/powacc.h"
#include <math.h>
#include <stdint.h>

/* xʸ = 2^(y·log₂x), correctly rounded.
 *
 * Ported from metallic-rs f64/pow.rs.  For now the magnitude is computed by the
 * CORE-MATH accurate cascade (kernel/powacc.h) on every non-edge input; the
 * lean Ziv fast path that fronts it in metallic-rs is a later optimization. */

/* xʸ for finite positive x ≠ 1, correctly rounded. */
static double pow_core_(double x, double y)
{
    /* Infinite exponent: e = y·log₂x = ±∞ with sign sign(y)·sign(x−1). */
    if (!isfinite(y))
        return (y > 0.0) == (x > 1.0) ? HUGE_VAL : 0.0;

    /* Gross over/underflow gate.  The accurate path mirrors CORE-MATH, which
     * relies on a fast path to pre-filter the gross cases (its exp_3 has no
     * over/underflow clamp).  log2 is correctly rounded here, so e is good to
     * ~1 ulp and the wide margins leave every boundary case to the accurate
     * path to round exactly.
     * ponytail: temporary — the Ziv fast path (pow_fast) will subsume this. */
    double e = y * log2(x);
    if (e > 1030.0)
        return HUGE_VAL;
    if (e < -1080.0)
        return 0.0;

    return pow_accurate_(x, y, x, 1.0);
}

/* |x|ʸ with the Annex F edge cases, treating x as its magnitude. */
static double magnitude_(double x, double y)
{
    if (isnan(x))
        return NAN;

    if (isinf(x)) {
        if (isnan(y))
            return NAN;
        return y > 0.0 ? HUGE_VAL : (y < 0.0 ? 0.0 : 1.0);
    }

    if (x == 0.0) {
        if (isnan(y))
            return NAN;
        return y > 0.0 ? 0.0 : (y < 0.0 ? HUGE_VAL : 1.0);
    }

    if (x == 1.0)
        return 1.0; /* 1ʸ = 1 for every y, including NaN */

    if (signbit(x) || isnan(y))
        return NAN; /* negative base (complex result) or NaN exponent, x ≠ 1 */

    return pow_core_(x, y);
}

double pow(double x, double y)
{
    if (y == 0.0)
        return 1.0; /* xʸ = 1 for every x, including NaN */

    /* Negative base with integer exponent: fold the sign out and raise |x|. */
    if (signbit(x) && trunc(y) == y) {
        double sign = (trunc(0.5 * y) != 0.5 * y) ? -1.0 : 1.0;
        return sign * magnitude_(-x, y);
    }

    return magnitude_(x, y);
}

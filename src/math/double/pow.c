#include "kernel/logtab.h" /* logtab_ln_dd_ + the exptab_* exp kernel + shift_ */
#include "kernel/powacc.h"
#include <math.h>
#include <stdint.h>

/* xʸ = exp(y·ln x), correctly rounded.
 *
 * Ported from metallic-rs f64/pow.rs.  A lean double-double 2^(y·log₂x) leg
 * under a Ziv gate (here exp(y·ln x), reusing the shared ln/exp dd kernels)
 * handles the vast majority of inputs; the rare straddles and the
 * over/underflow / subnormal edges defer to the CORE-MATH accurate cascade
 * (kernel/powacc.h). */

/* Reduced-argument exp boundaries in natural-log units (= k·ln2). */
#define POW_OVF 710.4758600739439   /* 1025·ln2: gross overflow → +∞ */
#define POW_UNF (-745.1332191019412) /* −1075·ln2: gross underflow → 0 */
#define POW_HI 709.0895657128241    /* 1023·ln2: top of the fast normal band */
#define POW_LO (-708.3964185322641) /* −1022·ln2: bottom of the fast band */

/* Fast leg: exp(y·ln x) as a double-double accepted by a Ziv gate.  Writes the
 * result to *out and returns 1 when resolved, else 0 (defer to the accurate
 * path).  x is finite positive ≠ 1, y finite. */
static int pow_fast_(double x, double y, double *out)
{
    exptab_sum_ l = logtab_ln_dd_(x); /* ln x, |error| ≤ 2⁻⁷⁸ */

    /* slack bounds the [1,2) mantissa error: ln's ≤2⁻⁷⁸ absolute slip, scaled by
     * y across the exp, gives ≲|y|·2⁻⁷⁷; (1+|y|)·2⁻⁷⁴ keeps a wide margin. */
    double slack = (1.0 + fabs(y)) * 0x1p-74;

    /* de ≈ y·ln x decides gross over/underflow directly (it also catches a
     * ±∞ product from a huge y).  Because |ln x| ≥ ~2⁻⁵³ for x ≠ 1, any de left
     * in range forces |y| ≲ 2⁶³, so the double-double product below cannot
     * overflow its Dekker split. */
    double de = y * l.hi;
    if (de - slack > POW_OVF) {
        *out = HUGE_VAL;
        return 1;
    }
    if (de + slack < POW_UNF) {
        *out = 0.0;
        return 1;
    }

    /* Leave the over/underflow boundary bands and the subnormal range to the
     * accurate path, which rounds them exactly. */
    if (!(de + slack < POW_HI && de - slack > POW_LO))
        return 0;

    /* e = y·ln x as a double-double. */
    exptab_sum_ p = exptab_prod_(l.hi, y);
    exptab_sum_ e = exptab_fast2sum_(p.hi, p.lo + l.lo * y);

    /* Reduce e to r = e − n·ln2/128, n = 128·q + j; |r| ≤ ln2/256. */
    double scaled = rint(e.hi * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;
    int j = n & (EXPTAB_N - 1);
    int64_t q = n >> 7;

    double a = e.hi - scaled * exptab_ln2_over_n_hi_;
    exptab_sum_ blo = exptab_prod_(scaled, exptab_ln2_over_n_lo_);
    exptab_sum_ r = exptab_twosum_(a, -blo.hi);
    r = exptab_fast2sum_(r.hi, (r.lo + e.lo) - blo.lo - scaled * exptab_ln2_over_n_lo2_);

    exptab_sum_ m = exptab_mantissa_(j, &q, r); /* 2^(j/128)·exp(r) ∈ [1,2) */

    /* Ziv test: accept when both ends of the ±slack interval round alike and
     * the result is comfortably normal. */
    double lo = m.hi + (m.lo - slack);
    double hi = m.hi + (m.lo + slack);
    if (lo == hi && q >= -1021 && q <= 1022) {
        *out = shift_(lo, q);
        return 1;
    }
    return 0;
}

/* xʸ for finite positive x ≠ 1, correctly rounded. */
static double pow_core_(double x, double y)
{
    /* Infinite exponent: e = y·log₂x = ±∞ with sign sign(y)·sign(x−1). */
    if (!isfinite(y))
        return (y > 0.0) == (x > 1.0) ? HUGE_VAL : 0.0;

    double r;
    if (pow_fast_(x, y, &r))
        return r;

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

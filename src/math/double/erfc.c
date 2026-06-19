#include "kernel/erf.h"
#include <math.h>

/* The complementary error function erfc(x) = 1 − erf(x), correctly rounded.
 * A lean fast leg runs first, Ziv-gated against the accurate path. */
double erfc(double x)
{
    if (isnan(x))
        return x;

    /* erfc → 2 for very negative x (covering −∞) and → +0 for large x (+∞). */
    if (x < -ERF_ONE_THRESHOLD)
        return 2.0;
    if (x > ERFC_ZERO_THRESHOLD)
        return 0.0;

    /* erfc is asymmetric; the hard cases are keyed on the raw (signed) input. */
    double wc = erf_db_(erfc_hard_, ERFC_HARD_N, x);
    if (wc)
        return wc;

    double ax = fabs(x);

    /* |x| < 0.4375: erfc(x) = 1 − erf(x), anchored at the exact 1.0. */
    if (ax < 0.4375)
        return erf_round_anchored_(1.0, erf_neg_(erf_small_dd_eval_(x, 0)));

    int seg = erfc_segment_(ax);
    int64_t q;
    sum_t m = erfc_eval_(ax, seg, 1, &q);

    if (x > 0.0) {
        /* erfc spans the full underflow range; gate the mantissa, but keep the
         * normal/subnormal boundary (q < −1021) on the accurate path. */
        if (q >= -1021) {
            double lo = m.hi + (m.lo - ERFC_ZIV_EPS);
            double hi = m.hi + (m.lo + ERFC_ZIV_EPS);
            if (lo == hi)
                return shift_(lo, q);
        }
        m = erfc_eval_(ax, seg, 0, &q);
        return erf_round_general_(m, q);
    }

    /* x < 0: erfc(x) = 2 − erfc(|x|) ∈ [1.46, 2), no cancellation. */
    sum_t e = exptab_add_((sum_t){ 2.0, 0.0 }, erf_neg_(erf_scale_(m, q)));
    double eps = fabs(e.hi) * ERF_REFLECT_EPS;
    double lo = e.hi + (e.lo - eps), hi = e.hi + (e.lo + eps);
    if (lo == hi)
        return lo;

    m = erfc_eval_(ax, seg, 0, &q);
    e = exptab_add_((sum_t){ 2.0, 0.0 }, erf_neg_(erf_scale_(m, q)));
    return e.hi + e.lo;
}

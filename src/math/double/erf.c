#include "kernel/erf.h"
#include <math.h>

/* The error function, correctly rounded.  Odd: erf(−x) = −erf(x).
 *
 * Each branch runs a lean fast leg first, accepted unless its error interval
 * straddles a rounding boundary (Ziv); otherwise the accurate path resolves it. */
double erf(double x)
{
    if (isnan(x))
        return x;

    double ax = fabs(x);

    /* |x| < 0.4375: erf(x) = x·P(x²) directly. */
    if (ax < 0.4375) {
        if (ax < 0x1p-1000)
            return erf_tiny_(x);

        sum_t e = erf_small_dd_eval_(x, 1);
        double eps = fabs(e.hi) * ERF_SMALL_EPS;
        double lo = e.hi + (e.lo - eps), hi = e.hi + (e.lo + eps);
        if (lo == hi)
            return lo;
        return erf_small_accurate_(x);
    }

    /* erf rounds to ±1 (covering ±∞) once erfc drops below ½ ulp. */
    if (ax > ERF_ONE_THRESHOLD)
        return copysign(1.0, x);

    /* erf is odd, so the hard cases are keyed on |x|. */
    double wc = erf_db_(erf_hard_, ERF_HARD_N, ax);
    if (wc)
        return copysign(wc, x);

    /* erf(x) = sign(x)·(1 − erfc(|x|)); erfc ∈ (0, 0.536] here, no cancellation. */
    int seg = erfc_segment_(ax);
    int64_t q;
    sum_t m = erfc_eval_(ax, seg, 1, &q);
    sum_t e = exptab_add_((sum_t){ 1.0, 0.0 }, erf_neg_(erf_scale_(m, q)));
    double eps = fabs(e.hi) * ERF_REFLECT_EPS;
    double lo = e.hi + (e.lo - eps), hi = e.hi + (e.lo + eps);
    if (lo == hi)
        return copysign(lo, x);

    m = erfc_eval_(ax, seg, 0, &q);
    e = exptab_add_((sum_t){ 1.0, 0.0 }, erf_neg_(erf_scale_(m, q)));
    return copysign(e.hi + e.lo, x);
}

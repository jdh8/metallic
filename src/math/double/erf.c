#include "kernel/erf.h"
#include <math.h>

/* The error function, correctly rounded.  Odd: erf(−x) = −erf(x). */
double erf(double x)
{
    if (isnan(x))
        return x;

    double ax = fabs(x);

    /* |x| < 0.4375: erf(x) = x·P(x²) directly (result-anchored). */
    if (ax < 0.4375) {
        if (ax < 0x1p-1000)
            return erf_tiny_(x);
        return erf_small_accurate_(x);
    }

    /* erf rounds to ±1 (covering ±∞) once erfc drops below ½ ulp. */
    if (ax > ERF_ONE_THRESHOLD)
        return copysign(1.0, x);

    /* erf is odd, so the hard cases are keyed on |x|. */
    double wc = erf_db_(erf_hard_, ERF_HARD_N, ax);
    if (wc)
        return copysign(wc, x);

    /* erf(x) = sign(x)·(1 − erfc(|x|)); erfc ∈ (0, 0.536] here, so 1 − erfc has
     * no cancellation and the result is always normal. */
    double anchor;
    int n;
    const sum_t *c = erfc_segment_(ax, &anchor, &n);
    int64_t q;
    sum_t m = erfc_eval_(ax, anchor, c, n, &q);
    sum_t r = exptab_add_((sum_t){ 1.0, 0.0 }, erf_neg_(erf_scale_(m, q)));

    return copysign(r.hi + r.lo, x);
}

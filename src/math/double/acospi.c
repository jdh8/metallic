/* C23 acospi: acos(x)/pi, correctly rounded.
 *
 * Port of metallic-rs `acospi` (src/f64_/atan.rs ~ line 1933), re-tailored
 * to lift this repo's certified acos setup by 1/pi (kernel/invpi.h): the
 * double-double pi/2 -+ asin_dd_ leg gains one 1/pi product before its Ziv
 * gate.  acospi(1) = +0, acospi(-1) = 1, and acospi(0) = 1/2 exactly; no
 * tiny band is needed for the result scale (it never goes subnormal -- the
 * minimum is ~2^-27.8 at the domain edge), but the |x| < 2^-27 band keeps
 * acos.c's dedicated pi/2 - x double-double with its tighter gate.
 *
 * The accurate tier deliberately does NOT lift acos.c's dint tier: that one
 * computes pi/2 - asin_dint(x), whose subtraction cancels up to ~29 leading
 * bits as x -> 1 (asin_dint_ itself computes pi/2 - 2*atan(w) there, so the
 * round trip through two pi/2 offsets leaves only ~2^-99 relative accuracy
 * -- not enough for acos's ~2^-111 corpus ties).  Instead the reflection
 * acos(x) = 2*atan(sqrt((1-x)/(1+x))) is evaluated directly in dint, which
 * is cancellation-free (the doubling is an exponent bump). */

#include "kernel/asin.h"
#include "kernel/invpi.h"
#include <math.h>

/* Fast-leg relative error bound: acos.c's 2-ulp relative gate (ACOS_ERR
 * there) carried over unchanged; a relative bound is scale-invariant under
 * the 1/pi lift, and the lift product's |w|*2^-102 sits far inside it. */
#define ACOSPI_ERR(w_hi) ((w_hi) * 0x1p-51)

/* pi as a dint: atantab's pi/2 significand with the exponent bumped. */
static const dint_t acospi_pi_dint_ = { 0, 1,
    ((unsigned __int128)0xc90fdaa22168c234ULL << 64) | 0xc4c6628b80dc1cd1ULL };

/* Accurate tier: acos(x)/pi in the 128-bit dint, cancellation-free. */
static double acospi_accurate_(double ax, int negative)
{
    dint_t r;

    if (ax <= 0.5) {
        /* pi/2 -+ asin(ax): asin <= pi/6 here, so the subtraction loses at
         * most one leading bit -- no cancellation. */
        dint_t a = asin_dint_(ax);
        if (!negative)
            a.sgn = !a.sgn;
        r = dint_add_(&atantab_pi_2_dint_, &a);
    } else {
        /* acos(ax) = 2*atan(w), w = sqrt((1-ax)/(1+ax)) -- the reflection
         * evaluated directly (same reduction as asin_dint_xd_'s x > 0.5
         * branch, minus its pi/2 fold). */
        dint_t xd = dint_from_f64_(ax);
        dint_t neg_xd = { 1, xd.ex, xd.m };
        dint_t num = dint_add_(&atantab_one_dint_, &neg_xd);
        dint_t den = dint_add_(&atantab_one_dint_, &xd);
        dint_t rden = atantab_recip_dint_(&den);
        dint_t ratio = dint_mul_(&num, &rden);
        dint_t w = asin_sqrt_dint_(&ratio);
        dint_t at = atantab_atan_dint_xd_(&w, dint_to_f64_(&w));
        at.ex += 1; /* 2*atan(w), exact */

        if (negative) {
            /* acos(-ax) = pi - 2*atan(w); 2*atan(w) < pi/2 -- no cancellation. */
            at.sgn = 1;
            r = dint_add_(&acospi_pi_dint_, &at);
        } else {
            r = at;
        }
    }

    dint_t w = dint_mul_(&r, &invpi_dint_);
    return dint_to_f64_(&w);
}

double acospi(double x)
{
    if (x != x)
        return x + x;

    double ax = fabs(x);

    /* Domain: [-1, 1].  |x| > 1 is a domain error returning NaN. */
    if (ax > 1.0)
        return (x - x) / (x - x);

    /* acospi(1) = +0; acospi(-1) = 1 exactly. */
    if (x == 1.0)
        return 0.0;
    if (x == -1.0)
        return 1.0;

    /* |x| < 2^-27: acos.c's pi/2 - x double-double band, lifted.  Its gate
     * (the r_lo rounding, the dropped cubic, and pi/2's third word) scales
     * by 1/pi, and the lift product adds a relative 2^-102. */
    if (ax < 0x1p-27) {
        double r_hi = atantab_pi_2_hi_ - x;
        double r_lo = (atantab_pi_2_hi_ - r_hi) - x + atantab_pi_2_lo_;
        exptab_sum_ w = exptab_mul_((exptab_sum_){ r_hi, r_lo },
                                    (exptab_sum_){ invpi_hi_, invpi_lo_ });
        double err = (fabs(r_lo) * 0x1p-52 + ax * ax * ax + 0x1p-100) * invpi_hi_
                   + w.hi * 0x1p-102;
        double left = w.hi + (w.lo - err);
        double right = w.hi + (w.lo + err);

        if (left == right)
            return left;

        return acospi_accurate_(ax, x < 0.0);
    }

    /* General case: |x| in [2^-27, 1), mirroring acos.c's dd setup. */
    int negative = x < 0.0;
    exptab_sum_ r;

    if (!negative) {
        /* acos(x) = pi/2 - asin(x) in double-double. */
        exptab_sum_ a = asin_dd_(ax);
        double r_hi = atantab_pi_2_hi_ - a.hi;
        double r_lo = (atantab_pi_2_hi_ - r_hi) - a.hi; /* residual */
        r_lo += atantab_pi_2_lo_ - a.lo;
        r = (exptab_sum_){ r_hi, r_lo };
    } else {
        /* acos(-ax) = pi/2 + asin(ax) in double-double. */
        exptab_sum_ a = asin_dd_(ax);
        exptab_sum_ pi_2 = { atantab_pi_2_hi_, atantab_pi_2_lo_ };
        r = exptab_add_(pi_2, a);
    }

    exptab_sum_ w = exptab_mul_(r, (exptab_sum_){ invpi_hi_, invpi_lo_ });
    double eps = ACOSPI_ERR(w.hi);
    double left = w.hi + (w.lo - eps);
    double right = w.hi + (w.lo + eps);

    if (left != right)
        return acospi_accurate_(ax, negative);

    return left;
}

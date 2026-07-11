/* C23 asinpi: asin(x)/pi, correctly rounded.
 *
 * Port of metallic-rs `asinpi` (src/f64_/atan.rs ~ line 2202), re-tailored
 * to lift this repo's certified asin engines by 1/pi (kernel/invpi.h): the
 * double-double asin_dd_ leg and the 128-bit asin_dint_ fallback each gain
 * one 1/pi product before their Ziv gate.  asinpi(+-1) = +-1/2 exactly.
 * The tiny band |x| < 2^-27 -- where asin(x) = x but asinpi(x) = x/pi still
 * has to round -- carries the exact Dekker product x*(1/pi) plus the +x^3/6
 * correction, and its deep tail (|x| < 2^-900, where the gate width itself
 * goes subnormal and the results eventually do) rides the 128-bit tier with
 * the subnormal-safe rounder outright (atan.rs ~ 2161). */

#include "kernel/asin.h"
#include "kernel/invpi.h"
#include <math.h>

/* Fast-leg relative error bound: asin.c's 2-ulp relative gate (ASIN_ERR
 * there) carried over unchanged -- a relative bound is scale-invariant
 * under the 1/pi lift, and the lift product's own |w|*2^-102 sits far
 * inside it. */
#define ASINPI_ERR(w_hi) ((w_hi) * 0x1p-51)

/* Tiny-band accurate tier: (x + x^3/6 + 3x^5/40)/pi in the 128-bit dint,
 * rounded by the subnormal-safe general finisher (metallic-rs
 * asinpi_tiny_accurate).  Underflowed correction products are sound to
 * drop: below ~2^-357 the terms sit beyond the dint's 127 bits. */
static double asinpi_tiny_accurate_(double x)
{
    /* 1/6 as a double-double (metallic-rs SIXTH, exact residual). */
    exptab_sum_ sixth = { 0x1.5555555555555p-3, 0x1.5555555555555p-57 };
    exptab_sum_ xx = exptab_prod_(x, x);
    exptab_sum_ factor = exptab_add_(sixth, (exptab_sum_){ xx.hi * 0.075, 0.0 });
    exptab_sum_ c = exptab_mul_(exptab_mul_(xx, (exptab_sum_){ x, 0.0 }), factor);

    dint_t xd = dint_from_f64_(x);
    dint_t w = dint_mul_(&xd, &invpi_dint_);

    if (c.hi != 0.0) {
        dint_t t = dint_from_f64_(c.hi);
        t = dint_mul_(&t, &invpi_dint_);
        w = dint_add_(&w, &t);
    }
    if (c.lo != 0.0) {
        dint_t t = dint_from_f64_(c.lo);
        t = dint_mul_(&t, &invpi_dint_);
        w = dint_add_(&w, &t);
    }
    return dint_to_f64_general_(&w);
}

/* Main-band accurate tier: the 128-bit asin lifted by the 128-bit 1/pi.
 * |x| >= 2^-27 keeps the result normal, as dint_to_f64_ requires. */
static double asinpi_accurate_(double ax)
{
    dint_t r = asin_dint_(ax);
    dint_t w = dint_mul_(&r, &invpi_dint_);
    return dint_to_f64_(&w);
}

double asinpi(double x)
{
    if (x != x)
        return x + x;

    double ax = fabs(x);

    /* Domain: [-1, 1].  |x| > 1 is a domain error returning NaN. */
    if (ax > 1.0)
        return (x - x) / (x - x);

    /* asinpi(+-1) = +-1/2 exactly. */
    if (ax == 1.0)
        return copysign(0.5, x);

    /* Tiny band: asin(x) rounds to x here (asin.c), but the 1/pi lift still
     * has to round x/pi + x^3/(6 pi) + ... */
    if (ax < 0x1p-27) {
        if (ax == 0)
            return x;
        if (ax < 0x1p-900)
            return asinpi_tiny_accurate_(x);

        /* Exact Dekker x*(1/pi) plus the second limb and the cubic; the
         * certified relative gate is 2^-102 (metallic-rs asinpi tiny leg,
         * atan.rs ~ 2218; the unfused cubic adds only ~2^-107). */
        exptab_sum_ z = exptab_prod_(invpi_hi_, x);
        double zl = z.lo + invpi_lo_ * x;
        zl += z.hi * (x * x) * (1.0 / 6);
        double eps = fabs(z.hi) * 0x1p-102;
        double lb = z.hi + (zl - eps);
        double ub = z.hi + (zl + eps);

        if (lb == ub)
            return lb;

        return asinpi_tiny_accurate_(x);
    }

    exptab_sum_ r = asin_dd_(ax);
    exptab_sum_ w = exptab_mul_(r, (exptab_sum_){ invpi_hi_, invpi_lo_ });
    double eps = ASINPI_ERR(w.hi);
    double left = w.hi + (w.lo - eps);
    double right = w.hi + (w.lo + eps);

    if (left != right)
        return copysign(asinpi_accurate_(ax), x);

    return copysign(left, x);
}

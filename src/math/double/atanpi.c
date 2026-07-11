/* C23 atanpi: atan(x)/pi, correctly rounded.
 *
 * Port of metallic-rs `atanpi` (src/f64_/atan.rs ~ line 1705), re-tailored
 * to lift this repo's certified atan engines by 1/pi (kernel/invpi.h): the
 * double-double atantab_atan_dd_ fast leg and the 128-bit atantab_atan_dint_
 * fallback each gain one 1/pi product before their Ziv gate.  The tiny band
 * |x| < 2^-27 -- where atan(x) = x but atanpi(x) = x/pi still has to round --
 * carries the exact Dekker product x*(1/pi) plus the -x^3/3 correction, and
 * its deep tail (|x| < 2^-900, where the gate width itself goes subnormal
 * and the results eventually do) rides the 128-bit tier with the
 * subnormal-safe rounder outright (atan.rs ~ 1576). */

#include "kernel/atantab.h"
#include "kernel/invpi.h"
#include <math.h>

/* Fast-leg absolute error bound: atan.c's certified 2^-58 (ATAN_ERR there,
 * ~2.8x over its empirical worst) scaled by 1/pi.  The lift product itself
 * is exact to a relative |w|*2^-102 (exptab_mul_ drops the lo*lo cross
 * terms), charged separately at the gate. */
#define ATANPI_ERR 0x1.45f306dc9c883p-60

/* Tiny-band accurate tier: (x - x^3/3 + x^5/5)/pi in the 128-bit dint,
 * rounded by the subnormal-safe general finisher (metallic-rs
 * atanpi_tiny_accurate).  For |x| below ~2^-357 the correction products
 * underflow to zero, which is sound: the dropped terms sit below 2^-714
 * relative, far beyond the dint's 127 bits. */
static double atanpi_tiny_accurate_(double x)
{
    /* -1/3 as a double-double (metallic-rs THIRD_NEG, exact residual). */
    exptab_sum_ third_neg = { -0x1.5555555555555p-2, -0x1.5555555555555p-56 };
    exptab_sum_ xx = exptab_prod_(x, x);
    exptab_sum_ factor = exptab_add_(third_neg, (exptab_sum_){ xx.hi * 0.2, 0.0 });
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

/* Main-band accurate tier: the 128-bit atan lifted by the 128-bit 1/pi.
 * |x| >= 2^-27 keeps the result normal (>= ~2^-28.7), as dint_to_f64_
 * requires. */
static double atanpi_accurate_(double ax)
{
    dint_t r = atantab_atan_dint_(ax);
    dint_t w = dint_mul_(&r, &invpi_dint_);
    return dint_to_f64_(&w);
}

double atanpi(double x)
{
    if (x != x)
        return x + x;

    double ax = fabs(x);

    /* atanpi(+-0) = +-0; atanpi(+-inf) = +-1/2. */
    if (ax == 0)
        return x;
    if (ax == INFINITY)
        return copysign(0.5, x);

    /* Tiny band: atan(x) rounds to x here (atan.c), but the 1/pi lift still
     * has to round x/pi - x^3/(3 pi) + ... */
    if (ax < 0x1p-27) {
        if (ax < 0x1p-900)
            return atanpi_tiny_accurate_(x);

        /* Exact Dekker x*(1/pi) plus the second limb and the cubic; the
         * certified relative gate is 2^-102 (metallic-rs atanpi_fast tiny
         * leg, atan.rs ~ 1628; the unfused cubic adds only ~2^-107). */
        exptab_sum_ z = exptab_prod_(invpi_hi_, x);
        double zl = z.lo + invpi_lo_ * x;
        zl += z.hi * (x * x) * (-1.0 / 3);
        double eps = fabs(z.hi) * 0x1p-102;
        double lb = z.hi + (zl - eps);
        double ub = z.hi + (zl + eps);

        if (lb == ub)
            return lb;

        return atanpi_tiny_accurate_(x);
    }

    exptab_sum_ r = atantab_atan_dd_(ax);
    exptab_sum_ w = exptab_mul_(r, (exptab_sum_){ invpi_hi_, invpi_lo_ });
    double eps = ATANPI_ERR + w.hi * 0x1p-102;
    double left = w.hi + (w.lo - eps);
    double right = w.hi + (w.lo + eps);

    if (left != right)
        return copysign(atanpi_accurate_(ax), x);

    return copysign(left, x);
}

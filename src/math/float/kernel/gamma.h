#ifndef METALLIC_KERNEL_GAMMA_FLOAT_H
#define METALLIC_KERNEL_GAMMA_FLOAT_H

/* Double-double machinery for a correctly-rounded tgammaf.
 *
 * tgammaf computes Gamma in double then rounds to float.  For float inputs the
 * double intermediate never overflows before the float result does (tgammaf
 * overflows at x ~ 35.04, far below DBL_MAX), so the whole task is to make the
 * double value accurate enough -- and to round it to float without a double
 * round -- so that every one of the 2^32 results is correctly rounded.
 *
 * The pieces below carry a hi+lo (double-double) pair through the algebraic
 * steps that feed the final float (the Lanczos rational sum, the exp2 scaling,
 * the final product, and the reflection division), then round the pair to the
 * nearest float with the residual breaking ties so the wide -> double -> float
 * path cannot double-round on a midpoint.
 *
 * The EFT primitives mirror the file-local mul_/add_ in src/math/double/fma.c;
 * fma() is a true correctly-rounded fused op on this target (no hardware FMA,
 * FP_FAST_FMA undefined), so it is used for the two-product residual. */

#include "../../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Two-sum: y = round(a+b), *r = exact error.  Knuth, no ordering required. */
static double gamma_twosum_(double a, double b, double r[static 1])
{
    double y = a + b;
    double s = y - a;

    *r = s - y + a + (b - s);
    return y;
}

/* Two-product: y = round(a*b), *r = exact error (via the true fused fma). */
static double gamma_twoprod_(double a, double b, double r[static 1])
{
    double y = a * b;

    *r = fma(a, b, -y);
    return y;
}

/* (a_hi + a_lo) * (b_hi + b_lo) -> (hi, *lo), double-double product. */
static double gamma_mul_dd_(double a_hi, double a_lo, double b_hi, double b_lo, double lo[static 1])
{
    double p_lo;
    double p_hi = gamma_twoprod_(a_hi, b_hi, &p_lo);

    p_lo += a_hi * b_lo + a_lo * b_hi;

    double hi = p_hi + p_lo;
    *lo = p_hi - hi + p_lo;
    return hi;
}

/* a / (b_hi + b_lo) -> (hi, *lo), double-double quotient of a scalar numerator
 * by a double-double denominator.  One Newton refinement on the residual. */
static double gamma_div_dd_(double a, double b_hi, double b_lo, double lo[static 1])
{
    double q = a / b_hi;

    /* residual = a - q*(b_hi + b_lo), computed with a fused product so the
     * leading term is exact. */
    double r_lo;
    double r_hi = gamma_twoprod_(q, b_hi, &r_lo);
    double residual = a - r_hi - r_lo - q * b_lo;

    double corr = residual / b_hi;

    double hi = q + corr;
    *lo = q - hi + corr;
    return hi;
}

/* Round a double-double (hi, lo) to the nearest float, breaking ties by the
 * residual so the wide -> double -> float path cannot double-round.
 *
 * The only hazard is when s = RN(hi + lo) lands exactly on a float midpoint:
 * the cast then ties-to-even, but the true value hi + lo lies on one definite
 * side of the midpoint.  Detect the midpoint magnitude-independently -- the
 * float ULP varies with the exponent, so a fixed bit mask will not do -- by
 * comparing s to the exact midpoint of its two surrounding floats, and resolve
 * the tie toward the residual e.  For every non-midpoint s (the common case)
 * the plain cast already rounds correctly. */
static float gamma_to_float_odd_(double hi, double lo)
{
    double e;
    double s = gamma_twosum_(hi, lo, &e);
    float rn = (float)s;

    if (e) {
        double ds = rn;

        if (ds != s) {
            /* Bracket s strictly between the floats a < s < b. */
            float a = ds < s ? rn : nextafterf(rn, -(float)INFINITY);
            float b = ds < s ? nextafterf(rn, (float)INFINITY) : rn;

            if (s == 0.5 * ((double)a + (double)b))
                return e > 0 ? b : a;
        }
    }

    return rn;
}

#endif

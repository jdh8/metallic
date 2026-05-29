/* Kernel for correctly-rounded double asin and acos.
 *
 * Algorithm:
 *   asin(x) = atan(x / sqrt(1-x^2))          for |x| <= 0.5
 *   asin(x) = pi/2 - 2*atan(sqrt((1-x)/(1+x))) for 0.5 < x <= 1
 *
 * The fast path computes in double-double using the atan table engine.
 * The accurate path computes entirely in 128-bit dint.
 *
 * Both branches reduce to atan of a small or moderate argument, reusing
 * atantab_atan_dd_ and atantab_atan_dint_ from atantab.h. */
#ifndef METALLIC_KERNEL_ASIN_H
#define METALLIC_KERNEL_ASIN_H

#include "atantab.h"
#include <math.h>

/* dint reciprocal square root: 1/sqrt(a) via Newton r <- r*(3 - a*r^2)/2.
 * Seeds from the double approximation; two iterations give >127 bits. */
static inline dint_t asin_rsqrt_dint_(const dint_t *a)
{
    double ad = dint_to_f64_(a);
    dint_t r = dint_from_f64_(1.0 / sqrt(ad));

    /* r <- r * (3 - a*r^2) / 2  (two Newton steps: 53 -> 106 -> >127 bits) */
    static const dint_t THREE = { 0, 1, ((unsigned __int128)0xc000000000000000ULL << 64) | 0x0000000000000000ULL };
    for (int k = 0; k < 2; ++k) {
        dint_t r2   = dint_mul_(&r, &r);
        dint_t ar2  = dint_mul_(a, &r2);
        dint_t neg  = { !ar2.sgn, ar2.ex, ar2.m };
        dint_t thr  = dint_add_(&THREE, &neg);   /* 3 - a*r^2 */
        r = dint_mul_(&r, &thr);
        /* divide by 2: decrement exponent */
        r.ex -= 1;
    }
    return r;
}

/* dint square root: sqrt(a) = a * rsqrt(a). */
static inline dint_t asin_sqrt_dint_(const dint_t *a)
{
    dint_t ir = asin_rsqrt_dint_(a);
    return dint_mul_(a, &ir);
}

/* Accurate asin for x > 0 given as a pre-computed dint (with double hint for
 * comparison branches).  This allows callers with a full 128-bit x to avoid
 * the precision loss of dint_from_f64_(double). */
static inline dint_t asin_dint_xd_(const dint_t *xd, double xd_);

/* Accurate asin for x > 0 (finite, not zero), using dint arithmetic.
 *
 * For x <= 0.5: asin(x) = atan(x / sqrt(1-x^2)).
 * For x >  0.5: asin(x) = pi/2 - 2*atan(sqrt((1-x)/(1+x))). */
static inline dint_t asin_dint_(double x)
{
    dint_t xd = dint_from_f64_(x);
    return asin_dint_xd_(&xd, x);
}

static inline dint_t asin_dint_xd_(const dint_t *xd, double xd_)
{
    if (xd_ <= 0.5) {
        /* y = x / sqrt(1 - x^2); use the full dint for atan to retain
         * the 128-bit precision of the dint y argument. */
        dint_t x2 = dint_mul_(xd, xd);
        dint_t neg_x2 = { !x2.sgn, x2.ex, x2.m };
        dint_t one_m_x2 = dint_add_(&atantab_one_dint_, &neg_x2);
        dint_t sq = asin_sqrt_dint_(&one_m_x2);
        dint_t rq = atantab_recip_dint_(&sq);
        dint_t y  = dint_mul_(xd, &rq);
        return atantab_atan_dint_xd_(&y, dint_to_f64_(&y));
    }
    else {
        /* w = sqrt((1-x)/(1+x)); asin(x) = pi/2 - 2*atan(w).
         * Use the full dint w for atan to retain 128-bit precision. */
        dint_t one_minus = { 0, 0, ((unsigned __int128)0x8000000000000000ULL << 64) | 0 };  /* 1 */
        dint_t neg_xd    = { !xd->sgn, xd->ex, xd->m };
        dint_t a = dint_add_(&one_minus, &neg_xd);   /* 1 - x */
        dint_t b = dint_add_(&one_minus, xd);        /* 1 + x */
        dint_t rb = atantab_recip_dint_(&b);
        dint_t ratio = dint_mul_(&a, &rb);
        dint_t w = asin_sqrt_dint_(&ratio);
        dint_t at = atantab_atan_dint_xd_(&w, dint_to_f64_(&w));
        /* 2 * atan(w) */
        dint_t two_at = dint_mul_int_(&at, 2);
        dint_t neg_two_at = { !two_at.sgn, two_at.ex, two_at.m };
        return dint_add_(&atantab_pi_2_dint_, &neg_two_at);
    }
}

/* Fast double-double asin for x > 0 (finite, not zero, |x| >= 2^-27).
 * Returns hi+lo such that |hi+lo - asin(x)| < ASIN_ERR.
 *
 * The key precision step: compute the atan argument to ~106 bits using the
 * sqrt correction c_lo = (v - c*c)/(2c) so that c + c_lo ≈ sqrt(v) exactly.
 * Then y = x/(c+c_lo) ≈ x/c - x*c_lo/c² to 106 bits, and the total fast-
 * path error stays well below 2^-58. */
static inline exptab_sum_ asin_dd_(double x)
{
    if (x <= 0.5) {
        /* y = x / sqrt(1 - x^2), then asin(x) = atan(y).
         * Use sqrt correction to get y accurate to ~106 bits. */
        double s   = x * x;
        double v   = 1.0 - s;
        double c   = sqrt(v);                    /* fl(sqrt(1-x^2)) */
        double c_lo = (v - c * c) / (2.0 * c);  /* sqrt correction: c + c_lo ≈ sqrt(v) */
        double y   = x / c;                      /* fl(x/c) */
        double y_lo = -(y * c_lo) / c;          /* first-order correction for c_lo */
        exptab_sum_ at = atantab_atan_dd_(y);
        at.lo += y_lo / (1.0 + y * y);          /* atan correction for y_lo */
        return at;
    }
    else {
        /* w = sqrt((1-x)/(1+x)); asin(x) = pi/2 - 2*atan(w).
         * Use sqrt correction for w too. */
        double num = 1.0 - x;
        double den = 1.0 + x;
        double ratio = num / den;
        double w    = sqrt(ratio);
        double w_lo = (ratio - w * w) / (2.0 * w);  /* sqrt correction */
        exptab_sum_ at = atantab_atan_dd_(w);
        at.lo += w_lo / (1.0 + w * w);          /* atan correction for w_lo */
        /* pi/2 - 2*atan(w) as double-double. */
        double r_hi = atantab_pi_2_hi_ - 2.0 * at.hi;
        double r_lo = (atantab_pi_2_hi_ - r_hi) - 2.0 * at.hi;
        r_lo += atantab_pi_2_lo_ - 2.0 * at.lo;
        return (exptab_sum_){ r_hi, r_lo };
    }
}

#endif /* METALLIC_KERNEL_ASIN_H */

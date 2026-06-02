#ifndef METALLIC_ORACLE_CMPFR_H
#define METALLIC_ORACLE_CMPFR_H

/* High-precision MPFR references for the complex-float correct-rounding oracle.
 *
 * Each part of a complex function is an elementary real function of the inputs,
 * so the reference computes it in MPFR at CMPFR_PREC bits (far beyond float) via
 * a numerically-stable identity, then rounds to float with mpfr_get_flt.  At
 * this precision the per-part rounding is a single 240->24-bit rounding, so the
 * result is the exact correctly-rounded float -- the ground truth the harness
 * compares against.  All routines use only local mpfr_t (no global emin state),
 * so they are safe to call in any order.
 *
 * float inputs convert exactly into the 240-bit registers (mpfr_set_flt). */

#include <complex.h>
#include <math.h>
#include <mpfr.h>

#define CMPFR_PREC 240

/* cabsf: re = hypot(x,y), im = +0. */
static inline void ref_cabsf(float x, float y, float *re, float *im)
{
    mpfr_t mx, my, r;
    mpfr_inits2(CMPFR_PREC, mx, my, r, (mpfr_ptr)0);
    mpfr_set_flt(mx, x, MPFR_RNDN);
    mpfr_set_flt(my, y, MPFR_RNDN);
    mpfr_hypot(r, mx, my, MPFR_RNDN);
    *re = mpfr_get_flt(r, MPFR_RNDN);
    *im = 0.0f;
    mpfr_clears(mx, my, r, (mpfr_ptr)0);
}

/* cargf: re = atan2(y,x), im = +0. */
static inline void ref_cargf(float x, float y, float *re, float *im)
{
    mpfr_t mx, my, r;
    mpfr_inits2(CMPFR_PREC, mx, my, r, (mpfr_ptr)0);
    mpfr_set_flt(mx, x, MPFR_RNDN);
    mpfr_set_flt(my, y, MPFR_RNDN);
    mpfr_atan2(r, my, mx, MPFR_RNDN);
    *re = mpfr_get_flt(r, MPFR_RNDN);
    *im = 0.0f;
    mpfr_clears(mx, my, r, (mpfr_ptr)0);
}

/* csqrtf principal branch.  re = sqrt((|z|+x)/2) >= 0, im = sign(y)*sqrt((|z|-x)/2).
 *
 * Computing the smaller component as a bare sqrt of a difference cancels
 * catastrophically (|z|-x for x>0, |z|+x for x<0) -- even 240 bits is not enough
 * near an axis, which would make the *reference* wrong.  Instead compute only
 * the larger component (always sqrt((|z|+|x|)/2), no cancellation) and derive
 * the smaller from the exact identity re*|im| = |y|/2. */
static inline void ref_csqrtf(float x, float y, float *re, float *im)
{
    mpfr_t mx, ax, ay, m, big, small;
    mpfr_inits2(CMPFR_PREC, mx, ax, ay, m, big, small, (mpfr_ptr)0);
    mpfr_set_flt(mx, x, MPFR_RNDN);
    mpfr_abs(ax, mx, MPFR_RNDN);
    mpfr_set_flt(ay, y, MPFR_RNDN);
    mpfr_abs(ay, ay, MPFR_RNDN);
    mpfr_hypot(m, mx, ay, MPFR_RNDN);

    /* big = sqrt((|z| + |x|) / 2): the larger output magnitude, no cancellation. */
    mpfr_add(big, m, ax, MPFR_RNDN);
    mpfr_div_2ui(big, big, 1, MPFR_RNDN);
    mpfr_sqrt(big, big, MPFR_RNDN);

    if (mpfr_zero_p(big)) {
        *re = 0.0f;
        *im = y;                 /* z == 0: csqrt(0) = 0 with imag sign of y */
    } else {
        /* small = |y| / (2*big), the smaller output magnitude. */
        mpfr_div(small, ay, big, MPFR_RNDN);
        mpfr_div_2ui(small, small, 1, MPFR_RNDN);

        if (signbit(x)) {
            *re = mpfr_get_flt(small, MPFR_RNDN);
            *im = copysignf(mpfr_get_flt(big, MPFR_RNDN), y);
        } else {
            *re = mpfr_get_flt(big, MPFR_RNDN);
            *im = copysignf(mpfr_get_flt(small, MPFR_RNDN), y);
        }
    }

    mpfr_clears(mx, ax, ay, m, big, small, (mpfr_ptr)0);
}

/* cexpf: re = exp(x)*cos(y), im = exp(x)*sin(y). */
static inline void ref_cexpf(float x, float y, float *re, float *im)
{
    mpfr_t mx, my, ex, c, s;
    mpfr_inits2(CMPFR_PREC, mx, my, ex, c, s, (mpfr_ptr)0);
    mpfr_set_flt(mx, x, MPFR_RNDN);
    mpfr_set_flt(my, y, MPFR_RNDN);
    mpfr_exp(ex, mx, MPFR_RNDN);
    mpfr_sin_cos(s, c, my, MPFR_RNDN);
    mpfr_mul(c, ex, c, MPFR_RNDN);
    mpfr_mul(s, ex, s, MPFR_RNDN);
    *re = mpfr_get_flt(c, MPFR_RNDN);
    *im = mpfr_get_flt(s, MPFR_RNDN);
    mpfr_clears(mx, my, ex, c, s, (mpfr_ptr)0);
}

#endif

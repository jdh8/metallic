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

#endif

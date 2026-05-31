/* Exhaustive correct-rounding oracle for lgammaf (= ln|Gamma(x)|).
 *
 * CORE-MATH ships no binary32 lgamma reference, so this provides its own,
 * following the binary32/support pattern: compute ln|Gamma| straight at
 * binary32 precision (24 bits) with round-to-nearest, which by MPFR's Ziv
 * strategy is the correctly-rounded binary32 result (single rounding, fast).
 *
 * sweep_f32 iterates all 2^32 binary32 inputs and compares bit-for-bit. */

#include "src/math/float/lgammaf.c"
#include "../../sweep.h"

#include <mpfr.h>
#include <math.h>

static void g_init(void)
{
    mpfr_set_emin(-148);
    mpfr_set_emax(128);
}

/* Correctly-rounded ln|Gamma(x)| in binary32. */
static float ref(float x)
{
    /* C / Annex F special cases:
     *   lgamma(1) = lgamma(2)    = +0
     *   lgamma(+-0)              = +inf (pole)
     *   lgamma(negative integer) = +inf (pole)
     *   lgamma(+-inf)            = +inf
     *   lgamma(NaN)              = NaN */
    if (isnan(x))
        return x;

    if (isinf(x))
        return (float)INFINITY;

    if (x == 0.0f)
        return (float)INFINITY;

    if (x < 0.0f && truncf(x) == x)
        return (float)INFINITY;

    mpfr_t y;
    mpfr_init2(y, 24);
    mpfr_set_flt(y, x, MPFR_RNDN);
    int sign;
    int inex = mpfr_lgamma(y, &sign, y, MPFR_RNDN);
    inex = mpfr_subnormalize(y, inex, MPFR_RNDN);
    (void)inex;
    float r = mpfr_get_flt(y, MPFR_RNDN);
    mpfr_clear(y);
    return r;
}

static float fut(float x) { return lgammaf(x); }

int main(void)
{
    return sweep_f32(fut, ref, g_init);
}

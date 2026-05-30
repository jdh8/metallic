/* Exhaustive correct-rounding oracle for tgammaf.
 *
 * CORE-MATH ships no binary32 gamma reference, so this provides its own,
 * following the binary32/support pattern (see erff_mpfr.c): compute Gamma
 * straight at binary32 precision (24 bits) with round-to-nearest.  By MPFR's
 * Ziv strategy that IS the correctly-rounded binary32 result -- the value is
 * rounded once, directly to the target precision, so there is no double round,
 * and the working precision stays small (fast).
 *
 * sweep_f32 iterates all 2^32 binary32 inputs and compares bit-for-bit. */

#include "src/math/float/tgammaf.c"
#include "../../sweep.h"

#include <mpfr.h>
#include <math.h>

/* Per-thread MPFR setup: binary32 exponent range so subnormals/overflow round
 * as binary32 does.  sweep_f32 runs this on every OpenMP worker (MPFR's
 * exponent range is thread-local). */
static void g_init(void)
{
    mpfr_set_emin(-148);
    mpfr_set_emax(128);
}

/* Correctly-rounded Gamma(x) in binary32. */
static float ref(float x)
{
    /* C / Annex F special cases (match CORE-MATH's ref_tgamma):
     *   tgamma(+-0)              = +-inf (pole)
     *   tgamma(-inf)             = NaN
     *   tgamma(+inf)             = +inf
     *   tgamma(NaN)              = NaN
     *   tgamma(negative integer) = NaN   (pole) */
    if (isnan(x))
        return x;

    if (isinf(x))
        return x > 0 ? (float)INFINITY : (float)NAN;

    if (x == 0.0f)
        return copysignf((float)INFINITY, x);

    if (x < 0.0f && truncf(x) == x)
        return (float)NAN;

    mpfr_t y;
    mpfr_init2(y, 24);
    mpfr_set_flt(y, x, MPFR_RNDN);          /* exact: 24-bit input */
    int inex = mpfr_gamma(y, y, MPFR_RNDN); /* correctly rounded to 24 bits */
    inex = mpfr_subnormalize(y, inex, MPFR_RNDN);
    (void)inex;
    float r = mpfr_get_flt(y, MPFR_RNDN);   /* exact in normal range */
    mpfr_clear(y);
    return r;
}

static float fut(float x) { return tgammaf(x); }

int main(void)
{
    return sweep_f32(fut, ref, g_init);
}

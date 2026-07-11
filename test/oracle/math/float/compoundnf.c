#include "src/math/float/compoundnf.c"
#include "../../sample.h"
#include <mpfr.h>

/* MPFR ground truth built from an exact 1 + x (a 300-bit sum holds every
 * finite float case exactly) through mpfr_pow.  mpfr_compound_si is avoided:
 * MPFR 4.2.2 misrounds it for large n (see the binary64 compoundn oracle). */

static float ref_compound_y_(float x, float y)
{
    static mpfr_t s, ym, r;
    static int init;
    if (!init) {
        mpfr_init2(s, 300);
        mpfr_init2(ym, 64);
        mpfr_init2(r, 24);
        init = 1;
    }
    if (x < -1.0f)
        return (x - x) / (x - x); /* mpfr_pow accepts a negative base with integer y */
    mpfr_set_d(s, (double)x, MPFR_RNDN);
    mpfr_add_ui(s, s, 1, MPFR_RNDN); /* exact at 300 bits */
    mpfr_set_d(ym, (double)y, MPFR_RNDN);
    int inex = mpfr_pow(r, s, ym, MPFR_RNDN);
    inex = mpfr_subnormalize(r, inex, MPFR_RNDN);
    return mpfr_get_flt(r, MPFR_RNDN);
}

static float ref_compoundnf_(float x, long long n)
{
    static mpfr_t s, ym, r;
    static int init;
    if (!init) {
        mpfr_init2(s, 300);
        mpfr_init2(ym, 64);
        mpfr_init2(r, 24);
        init = 1;
    }
    if (x < -1.0f)
        return (x - x) / (x - x);
    mpfr_set_d(s, (double)x, MPFR_RNDN);
    mpfr_add_ui(s, s, 1, MPFR_RNDN);
    mpfr_set_si(ym, (long)n, MPFR_RNDN);
    int inex = mpfr_pow(r, s, ym, MPFR_RNDN);
    inex = mpfr_subnormalize(r, inex, MPFR_RNDN);
    return mpfr_get_flt(r, MPFR_RNDN);
}

/* compound(x, y) with the full F.10.4.1 contract on the shared core, for the
 * real-exponent corpus. */
static float fut_y_(float x, float y)
{
    if (x == 0.0f)
        return 1.0f;
    if (y == 0.0f)
        return x < -1.0f ? (x - x) / (x - x) : 1.0f;
    if (x != x || y != y)
        return x + y;
    if (x < -1.0f)
        return (x - x) / (x - x);
    if (isinf(y))
        return (x > 0.0f) == (y > 0.0f) ? HUGE_VALF : 0.0f;
    if (x == INFINITY)
        return y > 0.0f ? HUGE_VALF : 0.0f;
    if (x == -1.0f)
        return y > 0.0f ? 0.0f : HUGE_VALF;
    if (y == 1.0f)
        return (float)(1.0 + (double)x);
    return compoundnf_core_((double)x, (double)y, 0.0);
}

/* Random integer exponents over independent x and n magnitude bands. */
static uint64_t band_n_(int xlo, int xhi, int nlo, int nhi, uint64_t count)
{
    uint64_t bad = 0, shown = 0;

    for (uint64_t k = 0; k < count; ++k) {
        float x = sample_draw_f_(xlo, xhi, 1);
        long long n = llrintf(sample_draw_f_(nlo, nhi, 1));
        float got = compoundnf(x, n);
        float want = ref_compoundnf_(x, n);
        int64_t d = sample_ulp_f_(got, want);

        if (!d)
            continue;

        ++bad;

        if (shown < 10) {
            ++shown;
            fprintf(stderr, "  x=%a n=%lld  got=%a  want=%a  (%lld ulp)\n",
                (double)x, n, (double)got, (double)want, (long long)d);
        }
    }

    fprintf(stderr, "  compoundnf x[2^%d,2^%d] n[2^%d,2^%d] x%llu: %llu bad\n",
        xlo, xhi, nlo, nhi, (unsigned long long)count, (unsigned long long)bad);
    return bad;
}

int main(void)
{
    uint64_t bad = 0;

    /* Match binary32 so mpfr_subnormalize rounds once. */
    mpfr_set_emin(-148);
    mpfr_set_emax(128);

    /* The CORE-MATH corpus (real exponents) through the shared core. */
    bad |= sample_wc_f32_2(fut_y_, ref_compound_y_,
        CORE_MATH "/binary32/compound/compoundf.wc");

    /* Integer exponents: the tiny-x large-n hard regime, moderate spreads,
     * subnormal x, the inexact-(double)n band (|n| > 2^53), big bases, and
     * x hugging -1. */
    bad |= band_n_(-10, 2, 1, 8, 3000000);
    bad |= band_n_(-30, -10, 5, 25, 3000000);
    bad |= band_n_(-60, -30, 25, 62, 3000000);
    bad |= band_n_(-149, -126, 1, 62, 2000000);
    bad |= band_n_(-24, -1, 54, 62, 1000000);
    bad |= band_n_(0, 30, 1, 4, 2000000);
    bad |= band_n_(-1, -1, 1, 7, 2000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}

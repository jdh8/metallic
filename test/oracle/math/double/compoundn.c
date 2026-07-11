#include "src/math/double/compoundn.c"
#include "../../sample.h"
#include <mpfr.h>

/* CORE-MATH has no binary64 compound, so the reference is built here: an
 * exact 1 + x (a 1200-bit sum holds every finite case exactly) through
 * mpfr_pow, with the exponent exact in 64 bits.  mpfr_compound_si is NOT the
 * oracle: MPFR 4.2.2 misrounds it for large n (e.g. compound_si of
 * x=0x1.6ccf7a3601c88p-60, n=-0x1.9c24ec8d1b1afp+61 returns ...cb24 where the
 * exact value rounds to ...cb25, confirmed by 80-digit decimal expansion). */

static double ref_compoundn_(double x, long long n)
{
    static mpfr_t s, ym, r;
    static int init;
    if (!init) {
        mpfr_init2(s, 1200);
        mpfr_init2(ym, 64);
        mpfr_init2(r, 53);
        init = 1;
    }
    if (x < -1.0)
        return NAN; /* mpfr_pow would accept a negative base with integer n */
    mpfr_set_d(s, x, MPFR_RNDN);
    mpfr_add_ui(s, s, 1, MPFR_RNDN); /* exact at 1200 bits */
    mpfr_set_si(ym, (long)n, MPFR_RNDN);
    int inex = mpfr_pow(r, s, ym, MPFR_RNDN);
    inex = mpfr_subnormalize(r, inex, MPFR_RNDN);
    return mpfr_get_d(r, MPFR_RNDN);
}

static double ref_compound_y_(double x, double y)
{
    static mpfr_t s, ym, r;
    static int init;
    if (!init) {
        mpfr_init2(s, 1200);
        mpfr_init2(ym, 53);
        mpfr_init2(r, 53);
        init = 1;
    }
    mpfr_set_d(s, x, MPFR_RNDN);
    mpfr_add_ui(s, s, 1, MPFR_RNDN); /* exact at 1200 bits */
    mpfr_set_d(ym, y, MPFR_RNDN);
    int inex = mpfr_pow(r, s, ym, MPFR_RNDN);
    inex = mpfr_subnormalize(r, inex, MPFR_RNDN);
    return mpfr_get_d(r, MPFR_RNDN);
}

/* The internal real-y pipeline (fast leg + accurate cascade), which the
 * f64_compound.wc corpus exercises beyond compoundn's integer exponents. */
static double fut_compound_y_(double x, double y)
{
    if (x <= -1.0 || x != x || y != y || y == 0.0 || y == 1.0 || isinf(x) || isinf(y))
        return ref_compound_y_(x, y); /* edges: not the pipeline under test */

    double r;
    if (compound_fast_(x, y, &r))
        return r;
    return compound_accurate_(x, y, 1, 0);
}

/* Integer-n sampling reuses the bivariate harness by rounding the drawn y
 * inside both the function under test and the reference. */
static double fut_n_(double x, double y) { return compoundn(x, llrint(y)); }
static double ref_n_(double x, double y) { return ref_compoundn_(x, llrint(y)); }

int main(void)
{
    uint64_t bad = 0;

    /* Match binary64 so mpfr_subnormalize rounds once (see pow.c oracle). */
    mpfr_set_emin(-1073);
    mpfr_set_emax(1024);

    /* The metallic-rs hardest-to-round corpus: all 2925 pairs through the
     * real-y pipeline, and the integer-exponent subset through compoundn. */
    bad |= sample_wc_f64_2(fut_compound_y_, ref_compound_y_,
        "test/oracle/math/double/cases/f64_compound.wc");
    bad |= sample_wc_f64_2(fut_n_, ref_n_,
        "test/oracle/math/double/cases/f64_compound.wc");

    /* Random integer exponents, x bands paired so results often stay finite:
     * the hard regime is tiny x with large n (the ×n amplification), then
     * moderate spreads, the subnormal-x corner, and the inexact-(double)n
     * band (|n| > 2^53). */
    bad |= sample_f64_2(fut_n_, ref_n_, -12, 2, 1, 10, 1, 4000000);
    bad |= sample_f64_2(fut_n_, ref_n_, -45, -12, 10, 35, 1, 4000000);
    bad |= sample_f64_2(fut_n_, ref_n_, -70, -45, 35, 62, 1, 4000000);
    bad |= sample_f64_2(fut_n_, ref_n_, -1074, -1022, 1, 62, 1, 2000000);
    bad |= sample_f64_2(fut_n_, ref_n_, -60, -40, 54, 62, 1, 2000000);

    /* x in ±[1/2, 1) (the negative half hugs x = −1), big bases, and the
     * overflow/underflow boundary bands. */
    bad |= sample_f64_2(fut_n_, ref_n_, -1, -1, 1, 6, 1, 2000000);
    bad |= sample_f64_2(fut_n_, ref_n_, 0, 60, 1, 4, 1, 2000000);
    bad |= sample_f64_2(fut_n_, ref_n_, 300, 1023, 1, 2, 1, 1000000);

    fputs(bad ? "FAIL\n" : "OK: correctly rounded over all sampled inputs\n", stderr);
    return bad != 0;
}

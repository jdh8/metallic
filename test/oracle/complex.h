#ifndef METALLIC_ORACLE_COMPLEX_H
#define METALLIC_ORACLE_COMPLEX_H

/* Sampling verification for binary32 complex functions.
 *
 * A complex float function is correctly rounded when each of its real and
 * imaginary parts is the correctly-rounded float of the exact mathematical
 * value (component-wise CR, as C / Annex G specify).  The 2^64 input domain
 * cannot be swept, so -- like the binary64 and bivariate-binary32 oracles --
 * this harness draws a large random sample (uniform exponent, full mantissa)
 * plus dedicated near-axis cases (y or x exactly 0, +-1, subnormal), compares
 * BOTH components bit-for-bit, and reports the worst ulp of each part
 * separately so a regression localises to one component.  A clean run is
 * strong evidence of correct rounding, not a proof.
 *
 * The reference (`cref_t`) computes each part in high-precision MPFR and rounds
 * to float; it uses only local mpfr_t (no global emin twiddling), so the
 * harness stays single-threaded for simplicity, matching sample.h. */

#include "sample.h"
#include <complex.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

typedef float _Complex cfut_t(float _Complex);
typedef void cref_t(float x, float y, float *re, float *im);

/* Check both parts of one input; sets *dre,*dim to the |ulp| of each part and
 * returns nonzero on any mismatch. */
static inline int cf_check_(cfut_t fut, cref_t ref, float x, float y,
                            int64_t *dre, int64_t *dim)
{
    float _Complex got = fut(CMPLXF(x, y));
    float wre, wim;
    ref(x, y, &wre, &wim);
    *dre = sample_ulp_f_(crealf(got), wre);
    *dim = sample_ulp_f_(cimagf(got), wim);
    return *dre || *dim;
}

static inline void cf_report_(const char *tag, float x, float y,
                              cfut_t fut, cref_t ref, int64_t dre, int64_t dim)
{
    float _Complex got = fut(CMPLXF(x, y));
    float wre, wim;
    ref(x, y, &wre, &wim);
    fprintf(stderr, "  %s x=%a y=%a  got=(%a,%a)  want=(%a,%a)  (re %lld, im %lld ulp)\n",
        tag, (double)x, (double)y,
        (double)crealf(got), (double)cimagf(got), (double)wre, (double)wim,
        (long long)dre, (long long)dim);
}

/* Draw `n` inputs with the real and imaginary exponents from independent bands
 * [exmin,exmax] and [eymin,eymax] and compare both parts to the oracle. */
static inline uint64_t sample_cf_box(cfut_t fut, cref_t ref,
                                     int exmin, int exmax, int eymin, int eymax,
                                     int signed_, uint64_t n)
{
    int64_t worst_re = 0, worst_im = 0;
    float wx = 0, wy = 0;
    uint64_t bad = 0, shown = 0;

    for (uint64_t k = 0; k < n; ++k) {
        float x = sample_draw_f_(exmin, exmax, signed_);
        float y = sample_draw_f_(eymin, eymax, signed_);
        int64_t dre, dim;

        if (!cf_check_(fut, ref, x, y, &dre, &dim))
            continue;

        ++bad;

        if (dre > worst_re || dim > worst_im) {
            if (dre > worst_re) worst_re = dre;
            if (dim > worst_im) worst_im = dim;
            wx = x;
            wy = y;
        }

        if (shown < 10) {
            ++shown;
            cf_report_("", x, y, fut, ref, dre, dim);
        }
    }

    fprintf(stderr, "  random re[2^%d,2^%d] im[2^%d,2^%d] x%llu: %llu bad,"
        " worst re %lld im %lld ulp at x=%a y=%a\n",
        exmin, exmax, eymin, eymax, (unsigned long long)n, (unsigned long long)bad,
        (long long)worst_re, (long long)worst_im, (double)wx, (double)wy);
    return bad;
}

/* Symmetric band: both parts drawn from [emin,emax] (so |x| ~ |y| is hit). */
static inline uint64_t sample_cf(cfut_t fut, cref_t ref,
                                 int emin, int emax, int signed_, uint64_t n)
{
    return sample_cf_box(fut, ref, emin, emax, emin, emax, signed_, n);
}

/* Near-axis cases: one part is pinned to a small/unit special (0, +-1,
 * subnormal, tiny) while the other ranges over [emin,emax]; both orientations
 * are exercised.  These are where the finite-core reformulations (log1p near
 * |z|=1, y/(2*re) in csqrt, ...) are most fragile. */
static inline uint64_t sample_cf_nearaxis(cfut_t fut, cref_t ref,
                                          int emin, int emax, uint64_t n)
{
    static const float pin[] = {
        0.0f, -0.0f, 1.0f, -1.0f, FLT_TRUE_MIN, -FLT_TRUE_MIN, FLT_MIN, -FLT_MIN,
    };
    int64_t worst_re = 0, worst_im = 0;
    float wx = 0, wy = 0;
    uint64_t bad = 0, shown = 0;

    for (uint64_t k = 0; k < n; ++k) {
        uint64_t r = sample_next_();
        float big = sample_draw_f_(emin, emax, 1);
        float small = pin[r & 7];
        float x, y;

        if (r & 8) { x = big; y = small; }
        else       { x = small; y = big; }

        int64_t dre, dim;

        if (!cf_check_(fut, ref, x, y, &dre, &dim))
            continue;

        ++bad;

        if (dre > worst_re || dim > worst_im) {
            if (dre > worst_re) worst_re = dre;
            if (dim > worst_im) worst_im = dim;
            wx = x;
            wy = y;
        }

        if (shown < 10) {
            ++shown;
            cf_report_("axis", x, y, fut, ref, dre, dim);
        }
    }

    fprintf(stderr, "  near-axis [2^%d,2^%d] x%llu: %llu bad,"
        " worst re %lld im %lld ulp at x=%a y=%a\n",
        emin, emax, (unsigned long long)n, (unsigned long long)bad,
        (long long)worst_re, (long long)worst_im, (double)wx, (double)wy);
    return bad;
}

#endif

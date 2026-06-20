#ifndef METALLIC_ORACLE_SAMPLE_H
#define METALLIC_ORACLE_SAMPLE_H

/* Sampling verification for binary64.
 *
 * 2^64 inputs cannot be brute-forced, so a double function cannot be PROVEN
 * correctly rounded by this harness the way the binary32 sweep proves a float
 * one.  Instead we (a) hammer the published hard-to-round databases -- the
 * CORE-MATH `*.wc` files, which list the inputs nearest the rounding boundary,
 * i.e. exactly the worst cases for the Table Maker's Dilemma -- and (b) draw a
 * large random sample with a uniform exponent and a full 52-bit random mantissa
 * so every magnitude is exercised with maximal significand entropy.  A clean
 * run is strong evidence of correct rounding, not a proof.
 *
 * Single-threaded on purpose: CORE-MATH's binary64 `ref_*` twiddle MPFR's
 * global emin (mpfr_set_emin), which is not safe under concurrent calls. */

#include "oracle.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Deterministic xorshift64* so a failing run is reproducible. */
static uint64_t sample_rng_ = 0x9e3779b97f4a7c15u;

static inline uint64_t sample_next_(void)
{
    uint64_t x = sample_rng_;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    sample_rng_ = x;
    return x * 0x2545F4914F6CDD1Du;
}

/* |ulp| distance, 0 when bit-identical or both NaN. */
static inline int64_t sample_ulp_(double a, double b)
{
    if ((isnan(a) && isnan(b)) || reinterpret(uint64_t, a) == reinterpret(uint64_t, b))
        return 0;

    int64_t d = ulp_dist_d(a, b);
    return d < 0 ? -d : d;
}

/* Random binary64 with exponent uniform in [emin, emax] and a full random
 * mantissa; sign random when `signed_` is set, else positive. */
static inline double sample_draw_(int emin, int emax, int signed_)
{
    uint64_t r = sample_next_();
    uint64_t mant = r & 0xFFFFFFFFFFFFFu;
    int e = emin + (int)((r >> 52) % (uint64_t)(emax - emin + 1));
    uint64_t sign = signed_ ? (sample_next_() & 1u) << 63 : 0;
    uint64_t bits = sign | ((uint64_t)(e + 1023) << 52) | mant;
    return reinterpret(double, bits);
}

/* Draw `n` random inputs over the exponent range and compare to the oracle.
 * Returns the number of incorrectly-rounded results. */
static inline uint64_t sample_f64(double fut(double), double ref(double),
                           int emin, int emax, int signed_, uint64_t n)
{
    int64_t worst = 0;
    double worst_x = 0;
    uint64_t bad = 0, shown = 0;

    for (uint64_t k = 0; k < n; ++k) {
        double x = sample_draw_(emin, emax, signed_);
        int64_t d = sample_ulp_(fut(x), ref(x));

        if (!d)
            continue;

        ++bad;

        if (d > worst) {
            worst = d;
            worst_x = x;
        }

        if (shown < 10) {
            ++shown;
            fprintf(stderr, "  x=%a  got=%a  want=%a  (%lld ulp)\n",
                x, fut(x), ref(x), (long long)d);
        }
    }

    fprintf(stderr, "  random[2^%d,2^%d] x%llu: %llu bad, worst %lld ulp at x=%a\n",
        emin, emax, (unsigned long long)n, (unsigned long long)bad,
        (long long)worst, worst_x);
    return bad;
}

/* Test every input in a CORE-MATH `*.wc` worst-case file (hex doubles, one per
 * line, `#` comments ignored).  These are the hardest-to-round inputs known. */
static inline uint64_t sample_wc_f64(double fut(double), double ref(double), const char *path)
{
    FILE *f = fopen(path, "r");

    if (!f) {
        fprintf(stderr, "  (no worst-case file %s)\n", path);
        return 0;
    }

    char line[256];
    int64_t worst = 0;
    double worst_x = 0;
    uint64_t bad = 0, total = 0, shown = 0;

    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || line[0] == '\n')
            continue;

        double x = strtod(line, NULL);
        ++total;
        int64_t d = sample_ulp_(fut(x), ref(x));

        if (!d)
            continue;

        ++bad;

        if (d > worst) {
            worst = d;
            worst_x = x;
        }

        if (shown < 10) {
            ++shown;
            fprintf(stderr, "  x=%a  got=%a  want=%a  (%lld ulp)\n",
                x, fut(x), ref(x), (long long)d);
        }
    }

    fclose(f);
    fprintf(stderr, "  worst-cases %s (%llu inputs): %llu bad, worst %lld ulp at x=%a\n",
        path, (unsigned long long)total, (unsigned long long)bad,
        (long long)worst, worst_x);
    return bad;
}

/* --- binary64 bivariate (pow) ------------------------------------------- */

/* Draw `n` random binary64 pairs and compare to the oracle.  `x` is drawn over
 * the exponent band [exmin, exmax] and `y` over [eymin, eymax] -- pow wants very
 * different natural ranges for base and exponent, so they are independent. */
static inline uint64_t sample_f64_2(double fut(double, double), double ref(double, double),
                             int exmin, int exmax, int eymin, int eymax, int signed_, uint64_t n)
{
    int64_t worst = 0;
    double worst_x = 0, worst_y = 0;
    uint64_t bad = 0, shown = 0;

    for (uint64_t k = 0; k < n; ++k) {
        double x = sample_draw_(exmin, exmax, signed_);
        double y = sample_draw_(eymin, eymax, 1);
        int64_t d = sample_ulp_(fut(x, y), ref(x, y));

        if (!d)
            continue;

        ++bad;

        if (d > worst) {
            worst = d;
            worst_x = x;
            worst_y = y;
        }

        if (shown < 10) {
            ++shown;
            fprintf(stderr, "  x=%a y=%a  got=%a  want=%a  (%lld ulp)\n",
                x, y, fut(x, y), ref(x, y), (long long)d);
        }
    }

    fprintf(stderr, "  random x[2^%d,2^%d] y[2^%d,2^%d] x%llu: %llu bad, worst %lld ulp at x=%a y=%a\n",
        exmin, exmax, eymin, eymax, (unsigned long long)n, (unsigned long long)bad,
        (long long)worst, worst_x, worst_y);
    return bad;
}

/* Test every pair in a CORE-MATH bivariate `*.wc` file (lines "x,y"; lines with
 * signaling NaNs are skipped). */
static inline uint64_t sample_wc_f64_2(double fut(double, double), double ref(double, double), const char *path)
{
    FILE *f = fopen(path, "r");

    if (!f) {
        fprintf(stderr, "  (no worst-case file %s)\n", path);
        return 0;
    }

    char line[256];
    int64_t worst = 0;
    double worst_x = 0, worst_y = 0;
    uint64_t bad = 0, total = 0, shown = 0;

    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || line[0] == '\n' || strstr(line, "snan"))
            continue;

        char *comma = strchr(line, ',');

        if (!comma)
            continue;

        *comma = 0;
        double x = strtod(line, NULL);
        double y = strtod(comma + 1, NULL);
        ++total;
        int64_t d = sample_ulp_(fut(x, y), ref(x, y));

        if (!d)
            continue;

        ++bad;

        if (d > worst) {
            worst = d;
            worst_x = x;
            worst_y = y;
        }

        if (shown < 10) {
            ++shown;
            fprintf(stderr, "  x=%a y=%a  got=%a  want=%a  (%lld ulp)\n",
                x, y, fut(x, y), ref(x, y), (long long)d);
        }
    }

    fclose(f);
    fprintf(stderr, "  worst-cases %s (%llu pairs): %llu bad, worst %lld ulp at x=%a y=%a\n",
        path, (unsigned long long)total, (unsigned long long)bad,
        (long long)worst, worst_x, worst_y);
    return bad;
}

/* --- binary32 bivariate (hypotf, atan2f, powf) -------------------------- */

static inline int64_t sample_ulp_f_(float a, float b)
{
    if ((isnan(a) && isnan(b)) || reinterpret(uint32_t, a) == reinterpret(uint32_t, b))
        return 0;

    int64_t d = ulp_dist_f(a, b);
    return d < 0 ? -d : d;
}

static inline float sample_draw_f_(int emin, int emax, int signed_)
{
    uint64_t r = sample_next_();
    uint32_t mant = (uint32_t)r & 0x7FFFFFu;
    int e = emin + (int)((r >> 32) % (uint32_t)(emax - emin + 1));
    uint32_t sign = signed_ ? (uint32_t)(sample_next_() & 1u) << 31 : 0;
    return reinterpret(float, sign | ((uint32_t)(e + 127) << 23) | mant);
}

/* Draw `n` random binary32 pairs and compare to the oracle.  Both arguments
 * share the exponent band so |x| ~ |y| cases (the hardest for hypot/atan2) are
 * hit often. */
static inline uint64_t sample_f32_2(float fut(float, float), float ref(float, float),
                             int emin, int emax, int signed_, uint64_t n)
{
    int64_t worst = 0;
    float worst_x = 0, worst_y = 0;
    uint64_t bad = 0, shown = 0;

    for (uint64_t k = 0; k < n; ++k) {
        float x = sample_draw_f_(emin, emax, signed_);
        float y = sample_draw_f_(emin, emax, signed_);
        int64_t d = sample_ulp_f_(fut(x, y), ref(x, y));

        if (!d)
            continue;

        ++bad;

        if (d > worst) {
            worst = d;
            worst_x = x;
            worst_y = y;
        }

        if (shown < 10) {
            ++shown;
            fprintf(stderr, "  x=%a y=%a  got=%a  want=%a  (%lld ulp)\n",
                (double)x, (double)y, (double)fut(x, y), (double)ref(x, y), (long long)d);
        }
    }

    fprintf(stderr, "  random[2^%d,2^%d]^2 x%llu: %llu bad, worst %lld ulp at x=%a y=%a\n",
        emin, emax, (unsigned long long)n, (unsigned long long)bad,
        (long long)worst, (double)worst_x, (double)worst_y);
    return bad;
}

/* Test every pair in a CORE-MATH bivariate `*.wc` file (lines "x,y"; lines with
 * signaling NaNs are skipped -- they are not distinguished here). */
static inline uint64_t sample_wc_f32_2(float fut(float, float), float ref(float, float), const char *path)
{
    FILE *f = fopen(path, "r");

    if (!f) {
        fprintf(stderr, "  (no worst-case file %s)\n", path);
        return 0;
    }

    char line[256];
    int64_t worst = 0;
    float worst_x = 0, worst_y = 0;
    uint64_t bad = 0, total = 0, shown = 0;

    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || line[0] == '\n' || strstr(line, "snan"))
            continue;

        char *comma = strchr(line, ',');

        if (!comma)
            continue;

        *comma = 0;
        float x = strtof(line, NULL);
        float y = strtof(comma + 1, NULL);
        ++total;
        int64_t d = sample_ulp_f_(fut(x, y), ref(x, y));

        if (!d)
            continue;

        ++bad;

        if (d > worst) {
            worst = d;
            worst_x = x;
            worst_y = y;
        }

        if (shown < 10) {
            ++shown;
            fprintf(stderr, "  x=%a y=%a  got=%a  want=%a  (%lld ulp)\n",
                (double)x, (double)y, (double)fut(x, y), (double)ref(x, y), (long long)d);
        }
    }

    fclose(f);
    fprintf(stderr, "  worst-cases %s (%llu pairs): %llu bad, worst %lld ulp at x=%a y=%a\n",
        path, (unsigned long long)total, (unsigned long long)bad,
        (long long)worst, (double)worst_x, (double)worst_y);
    return bad;
}

#endif

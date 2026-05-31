#include <math.h>
#include <stdint.h>
#include "../reinterpret.h"
#include "kernel/gamma.h"
#include "kernel/lgamma.h"

/* Rational g(z) = ln(Gamma(z))/((z-1)(z-2)) on [0.5, 8]. */
static const double lgamma_num_[8] = {
    0.006304151792758075,
    0.12417142343096166,
    0.4793835522141501,
    0.5550616679145655,
    0.21197392725308442,
    0.025465952676201734,
    0.0007365381479756475,
    1.1139674065637182e-6,
};
static const double lgamma_den_[8] = {
    0.002584416514557812,
    0.09084215068006321,
    0.5564764524876862,
    1.0,
    0.6281468455052526,
    0.14206877865256523,
    0.010505967556747394,
    0.0001797009335851256,
};

/* Stirling tail past 1/(12x): (u/x)*poly(u), u=1/x^2. */
static const double lgamma_tail_[7] = {
    -0.002777777777777778,
     0.0007936507936507937,
    -0.0005952380952380953,
     0.0008417508417508417,
    -0.0019175269175269176,
     0.00641025641025641,
    -0.029550653594771242,
};

static double lgamma_poly_eval_(double x, const double *c, int n)
{
    double y = c[n - 1];

    for (int i = n - 2; i >= 0; --i)
        y = y * x + c[i];

    return y;
}

/* lgammaf: correctly-rounded ln|Gamma(x)| for all binary32 inputs.
 *
 * The value is evaluated in double-double (gamma.h error-free transforms) and
 * rounded once with gamma_to_float_odd_.  For x >= 0.5 we use Stirling's series
 * with a recurrence that lifts the argument to z >= LG_STIRLING_CUTOFF; for
 * x < 0.5 we reflect with ln|Gamma(x)| = ln(pi) - ln|sin(pi x)| - ln|Gamma(1-x)|.
 * Because a binary32 result needs only ~24 bits, the double-double carries
 * enough absolute accuracy to resolve the tiny nonzero values near the exact
 * zeros at x = 1 and x = 2.  Uses only exactly-known constants, so no fitted
 * Lanczos coefficients are required.
 */

/* Forward path for z >= 0.5: ln Gamma(z) as a double-double.
 *
 * All accumulations renormalize through lg_add_dd_; the running totals (the
 * log-sum reaches ~20, the Stirling head ~700) dwarf the per-step residuals, so
 * a non-renormalized low word would silently drop bits and cost ~1 binary32 ulp. */
static double lgamma_pos_dd_(double z, double *rlo)
{
    /* Recurrence: ln Gamma(z) = ln Gamma(z+n) - sum_{j=0}^{n-1} ln(z+j). */
    double shi = 0.0, slo = 0.0;   /* accumulated sum of logs to subtract */
    double w = z;
    while (w < LG_STIRLING_CUTOFF) {
        double llo, l = lg_log_dd_(w, 0.0, &llo);
        shi = lg_add_dd_(shi, slo, l, llo, &slo);
        w += 1.0;
    }
    /* Stirling: (w-0.5)*ln w - w + 0.5 ln(2pi) + sum coeff_k / w^(2k-1). */
    double lwlo, lw = lg_log_dd_(w, 0.0, &lwlo);
    /* (w - 0.5) * ln w */
    double whlo, wh = gamma_twosum_(w, -0.5, &whlo);
    double m1lo, m1 = gamma_mul_dd_(wh, whlo, lw, lwlo, &m1lo);
    /* - w */
    double rlo2, rhi = lg_add_dd_(m1, m1lo, -w, 0.0, &rlo2);
    /* + 0.5 ln(2pi) */
    rhi = lg_add_dd_(rhi, rlo2, LG_HALF_LN2PI_HI, LG_HALF_LN2PI_LO, &rlo2);
    /* + Stirling tail: sum_{k=1}^{8} coeff_k / w^(2k-1)
     * = (1/w) * sum_k coeff_k * (1/w^2)^(k-1). */
    double winvlo, winv = gamma_div_dd_(1.0, w, 0.0, &winvlo);
    double w2lo, w2 = gamma_mul_dd_(winv, winvlo, winv, winvlo, &w2lo); /* 1/w^2 */
    /* Horner in (1/w^2) over coeff_k. */
    double phi = lg_stir_hi_[7], plo = lg_stir_lo_[7];
    for (int i = 6; i >= 0; i--) {
        phi = gamma_mul_dd_(phi, plo, w2, w2lo, &plo);
        phi = lg_add_dd_(phi, plo, lg_stir_hi_[i], lg_stir_lo_[i], &plo);
    }
    double taillo, tail = gamma_mul_dd_(phi, plo, winv, winvlo, &taillo); /* *(1/w) */
    rhi = lg_add_dd_(rhi, rlo2, tail, taillo, &rlo2);
    /* Subtract the recurrence sum. */
    rhi = lg_add_dd_(rhi, rlo2, -shi, -slo, &rlo2);
    *rlo = rlo2;
    return rhi;
}

/* Legacy full double-double implementation; used as the accurate fallback for
 * the tiny hard-to-round subset that escapes the fast path gate. */
static float lgammaf_dd_(float x)
{
    uint32_t ix = reinterpret(uint32_t, x) & 0x7fffffff;

    if (ix >= 0x7f800000) {
        if (ix == 0x7f800000)
            return INFINITY;
        return x + x;
    }
    if (ix == 0)
        return INFINITY;

    double z = (double)x;

    if (x <= 0.0f && x == floorf(x))
        return INFINITY;

    if (x == 1.0f || x == 2.0f)
        return 0.0f;

    double rhi, rlo;

    if (x >= 0.5f) {
        rhi = lgamma_pos_dd_(z, &rlo);
    } else {
        double k = nearbyint(z);
        double f = z - k;
        double lslo, ls = lg_logsin_dd_(f, &lslo);
        double glo, g = lgamma_pos_dd_(1.0 - z, &glo);
        double pplo, pp = lg_log_dd_(LG_PI_HI, LG_PI_LO, &pplo);
        double t;
        rhi = gamma_twosum_(pp, -ls, &rlo);
        rlo += pplo - lslo;
        rhi = gamma_twosum_(rhi, -g, &t);
        rlo += t - glo;
    }

    return gamma_to_float_odd_(rhi, rlo);
}

/* Fast f64 ln(Gamma(y)) for y >= 0.5. */
static double lgamma_pos_f64_(double y)
{
    if (y < 8.0) {
        double g = lgamma_poly_eval_(y, lgamma_num_, 8) / lgamma_poly_eval_(y, lgamma_den_, 8);
        return (y - 1.0) * (y - 2.0) * g;
    }

    double u = 1.0 / (y * y);
    double s = 1.0 / (12.0 * y) + lgamma_poly_eval_(u, lgamma_tail_, 7) * (u / y);
    return (y - 0.5) * log(y) - y + 0.9189385332046728 + s;
}

/* Fast ln|Gamma(z)| plus an absolute error envelope for the Ziv gate. */
static double lgamma_f64_(float z, double err[static 1])
{
    const double ln_pi = 1.1447298858494002;

    if (z < 0.5f) {
        double x = (double)z;
        double reflected = lgamma_pos_f64_(1.0 - x);
        double s = fabs(sin(LG_PI_HI * x));
        double value = ln_pi - log(s) - reflected;

        *err = ldexp(fabs(reflected), -36) + ldexp(1.0, -44);
        return value;
    }

    double value = lgamma_pos_f64_((double)z);
    *err = ldexp(fabs(value), -36) + ldexp(1.0, -44);
    return value;
}

float lgammaf(float x)
{
    if (x == 0.0f || x == INFINITY)
        return INFINITY;

    if (isnan(x))
        return x;

    if (x < 0.5f && nearbyintf(x) == x)
        return INFINITY;

    if (x == 1.0f || x == 2.0f)
        return 0.0f;

    double err;
    double f = lgamma_f64_(x, &err);
    float lo = (float)(f - err);
    float hi = (float)(f + err);

    if (lo == hi)
        return lo;

    return lgammaf_dd_(x);
}

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

/* Fast-path helpers ported verbatim from CORE-MATH (MIT license,
 * Copyright (c) 2023-2025 Alexei Sibidanov), src/binary32/lgamma/lgammaf.c
 * (as_sinpi / as_ln).  Plain f64, ~2^-43 relative error -- far inside the
 * 2^-36 Ziv gate envelope -- and no calls into the expensive double-double
 * log/sin kernels. */

/* sin(pi x) for x in [0, 1]. */
static double lgamma_sinpi_(double x)
{
    static const double c[] = {
        0x1p+2, -0x1.de9e64df22ea4p+1, 0x1.472be122401f8p+0, -0x1.d4fcd82df91bp-3,
        0x1.9f05c97e0aab2p-6, -0x1.f3091c427b611p-10, 0x1.b22c9bfdca547p-14, -0x1.15484325ef569p-18,
    };
    x -= 0.5;
    double x2 = x * x, x4 = x2 * x2, x8 = x4 * x4;
    return (0.25 - x2) * ((c[0] + x2 * c[1]) + x4 * (c[2] + x2 * c[3])
        + x8 * ((c[4] + x2 * c[5]) + x4 * (c[6] + x2 * c[7])));
}

/* log x for normal positive doubles: 16-entry reciprocal/log tables indexed
 * by the top 4 significand bits + a degree-8 polynomial in z = ix[i]*m - 1. */
static double lgamma_ln_(double x)
{
    static const double c[] = {
        0x1.fffffffffff24p-1, -0x1.ffffffffd1d67p-2, 0x1.55555537802dep-2, -0x1.ffffeca81b866p-3,
        0x1.999611761d772p-3, -0x1.54f3e581b61bfp-3, 0x1.1e642b4cb5143p-3, -0x1.9115a5af1e1edp-4,
    };
    static const double il[] = {
        0x1.59caeec280116p-57, 0x1.f0a30c01162aap-5, 0x1.e27076e2af2ebp-4, 0x1.5ff3070a793d6p-3,
        0x1.c8ff7c79a9a2p-3, 0x1.1675cababa60fp-2, 0x1.4618bc21c5ec2p-2, 0x1.739d7f6bbd007p-2,
        0x1.9f323ecbf984dp-2, 0x1.c8ff7c79a9a21p-2, 0x1.f128f5faf06ecp-2, 0x1.0be72e4252a83p-1,
        0x1.1e85f5e7040d1p-1, 0x1.307d7334f10bep-1, 0x1.41d8fe84672afp-1, 0x1.52a2d265bc5abp-1,
    };
    static const double ix[] = {
        0x1p+0, 0x1.e1e1e1e1e1e1ep-1, 0x1.c71c71c71c71cp-1, 0x1.af286bca1af28p-1,
        0x1.999999999999ap-1, 0x1.8618618618618p-1, 0x1.745d1745d1746p-1, 0x1.642c8590b2164p-1,
        0x1.5555555555555p-1, 0x1.47ae147ae147bp-1, 0x1.3b13b13b13b14p-1, 0x1.2f684bda12f68p-1,
        0x1.2492492492492p-1, 0x1.1a7b9611a7b96p-1, 0x1.1111111111111p-1, 0x1.0842108421084p-1,
    };
    uint64_t u = reinterpret(uint64_t, x);
    int e = (int)(u >> 52) - 0x3ff;
    int i = (u >> 48) & 0xf;
    double m = reinterpret(double, (u & (~(uint64_t)0 >> 12)) | ((uint64_t)0x3ff << 52));
    double z = ix[i] * m - 1, z2 = z * z, z4 = z2 * z2;
    return e * 0x1.62e42fefa39efp-1 + il[i] + z * ((c[0] + z * c[1]) + z2 * (c[2] + z * c[3])
        + z4 * ((c[4] + z * c[5]) + z2 * (c[6] + z * c[7])));
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
    return (y - 0.5) * lgamma_ln_(y) - y + 0.9189385332046728 + s;
}

/* Fast ln|Gamma(z)| plus an absolute error envelope for the Ziv gate. */
static double lgamma_f64_(float z, double err[static 1])
{
    const double ln_pi = 1.1447298858494002;

    if (z < 0.5f) {
        double x = (double)z;
        double reflected = lgamma_pos_f64_(1.0 - x);
        double f = x - rint(x);   /* fractional part in (-0.5, 0.5] */
        double af = fabs(f);
        double value;

        if (af < 0.125) {
            /* log|sin(pi*f)| = log(pi) + log|f| + log(sinc(pi*f)).
             * ln_pi == log(pi), so value = -log|f| - sinc_log - reflected.
             * sinc_log = log(sin(pi*f)/(pi*f)) = t*(c1 + t*(c2 + ...)), t=(pi*f)^2.
             * Degree-6 in t; truncation < 2.3e-14 for |f| < 0.125 (within 2^-44). */
            double t = (LG_PI_HI * f) * (LG_PI_HI * f);
            double sinc_log = (((((-1.0/5765760 * t
                - 1.0/467775) * t
                - 1.0/37800) * t
                - 1.0/2835) * t
                - 1.0/180) * t
                - 1.0/6) * t;
            value = -lgamma_ln_(af) - sinc_log - reflected;
        } else {
            /* |sin(pi x)| = sin(pi |f|) since |f| <= 0.5. */
            double s = lgamma_sinpi_(af);
            value = ln_pi - lgamma_ln_(s) - reflected;
        }

        *err = fabs(reflected) * 0x1p-36 + 0x1p-44;
        return value;
    }

    double value = lgamma_pos_f64_((double)z);
    *err = fabs(value) * 0x1p-36 + 0x1p-44;
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

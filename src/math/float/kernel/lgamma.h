#ifndef METALLIC_SRC_MATH_FLOAT_KERNEL_LGAMMA_H
#define METALLIC_SRC_MATH_FLOAT_KERNEL_LGAMMA_H

/* Double-double helpers for a correctly-rounded lgammaf.
 *
 * lgammaf must be correctly rounded for the binary32 result over all inputs,
 * including the exact zeros at x=1 and x=2 where ln|Gamma| is tiny.  Everything
 * is evaluated in double-double (the error-free transforms in gamma.h) and
 * rounded once with gamma_to_float_odd_.  A binary32 result needs only ~24 good
 * bits, but near x=1, x=2 and through the Stirling sum/recurrence cancellation
 * (the dip to ~-0.121 on [1,2]) the small result is a difference of larger
 * quantities, so the intermediate logs must carry full double-double precision.
 *
 * The evaluation is Stirling's asymptotic series with a recurrence that lifts
 * small arguments to z >= LG_STIRLING_CUTOFF, plus reflection for x < 0.5.  It
 * uses only exactly-known constants (Bernoulli rationals, ln 2pi, pi), so no
 * fitted Lanczos coefficients are required.
 */

#include <math.h>
#include "gamma.h"

/* Renormalizing double-double sum (a_hi+a_lo)+(b_hi+b_lo) -> (hi, *lo).
 * Knuth two-sum on the heads, then fold both low parts and re-normalize, so a
 * long chain of accumulations does not let the low word drift away from the
 * (growing) high word -- the precision leak a bare `lo += ...` would suffer. */
static double lg_add_dd_(double a_hi, double a_lo, double b_hi, double b_lo, double lo[static 1])
{
    double e;
    double s = gamma_twosum_(a_hi, b_hi, &e);
    e += a_lo + b_lo;
    double hi = s + e;
    *lo = s - hi + e;
    return hi;
}

/* Full double-double quotient (a_hi+a_lo)/(b_hi+b_lo).  gamma_div_dd_ takes a
 * scalar numerator, so divide the two numerator parts and recombine. */
static double lg_div_dd_(double a_hi, double a_lo, double b_hi, double b_lo, double lo[static 1])
{
    /* Briggs/Dekker double-double division.  q1 = a/b to ~53 bits; form the
     * residual a - q1*b in double-double (the product via the fused two-product
     * so its head is exact), then correct with q2 = residual/b. */
    double q1 = a_hi / b_hi;
    /* p = q1 * (b_hi + b_lo) as a double-double. */
    double plo, phi = gamma_twoprod_(q1, b_hi, &plo);
    plo += q1 * b_lo;
    /* residual r = (a_hi + a_lo) - p, in double-double. */
    double s_lo, s = gamma_twosum_(a_hi, -phi, &s_lo);
    s_lo += a_lo - plo;
    double q2 = (s + s_lo) / b_hi;
    double hi = q1 + q2;
    *lo = q1 - hi + q2;
    return hi;
}

/* 0.5*ln(2*pi) as a double-double. */
#define LG_HALF_LN2PI_HI 0x1.d67f1c864beb5p-1
#define LG_HALF_LN2PI_LO -0x1.65b5a1b7ff5dfp-55

/* pi as a double-double. */
#define LG_PI_HI 3.141592653589793
#define LG_PI_LO 1.2246467991473532e-16

/* ln(2) as a double-double. */
#define LG_LN2_HI 0.6931471805599453
#define LG_LN2_LO 2.3190468138462996e-17

/* Stirling coefficients B_{2k} / (2k*(2k-1)) as double-doubles, k = 1..8.
 *   ln Gamma(z) = (z-1/2) ln z - z + 1/2 ln(2pi) + sum_k coeff_k / z^(2k-1)
 * is asymptotic; we recurse so z >= LG_STIRLING_CUTOFF before summing, where
 * eight terms are far more accurate than double-double. */
static const double lg_stir_hi_[8] = {
     0x1.5555555555555p-4,
    -0x1.6c16c16c16c17p-9,
     0x1.a01a01a01a01ap-11,
    -0x1.3813813813814p-11,
     0x1.b951e2b18ff23p-11,
    -0x1.f6ab0d9993c7dp-10,
     0x1.a41a41a41a41ap-8,
    -0x1.e4286cb0f5398p-6,
};
static const double lg_stir_lo_[8] = {
     0x1.5555555555555p-58,
     0x1.f49f49f49f49fp-64,
     0x1.a01a01a01a01ap-71,
     0x1.fb1fb1fb1fb20p-65,
     0x1.5c3a9ce01b952p-65,
     0x1.f82553c999b0ep-64,
     0x1.0690690690690p-62,
     0x1.1efcdab896745p-61,
};

/* Recurse until z >= this before applying Stirling.  At z = 13 the first
 * omitted term (k=9, ~ 0.18/z^17) is ~ 1e-19 and shrinks fast. */
#define LG_STIRLING_CUTOFF 13.0

/* sin(pi*t) for |t| <= 0.5 as a double-double, via Taylor on the half range.
 * For |t| <= 0.25: sin(pi t) = (pi t) * S(t^2) with pi folded into the a_k.
 * Else:            sin(pi t) = cos(pi*(1/2 - |t|)) = C(u^2), u = 1/2 - |t| in
 *                  [0, 1/4], sign of t restored.  a_k = (-1)^k pi^(2k+1)/(2k+1)!,
 * b_k = (-1)^k pi^(2k)/(2k)!, split to dd. */
static const double lg_sinpi_a_hi_[8] = {
     3.141592653589793,    -5.16771278004997,      2.550164039877345,
    -0.5992645293207921,    0.08214588661112823,  -0.007370430945714350,
     0.0004663028057676932, -2.1915353447830217e-05,
};
static const double lg_sinpi_a_lo_[8] = {
     1.2246467991473532e-16, 2.2241363292326247e-16, -7.864671896773851e-17,
    -2.7064172561139924e-17, 2.7591435558369737e-18, -5.6884907021238545e-19,
     2.052498891805865e-20,  4.916628064464509e-22,
};
/* 14 terms: at the branch edge u = 1/2 - |f| -> 1/4 the first omitted term is
 * (pi/4)^28/28! ~ 2^-108, below the double-double round-off.  Eight terms left a
 * ~2^-50 truncation that leaked ~1 binary32 ulp into ln|sin| near |f| = 1/4. */
static const double lg_cospi_b_hi_[14] = {
    0x1.0000000000000p+0, -0x1.3bd3cc9be45dep+2, 0x1.03c1f081b5ac4p+2,
    -0x1.55d3c7e3cbffap+0, 0x1.e1f506891babbp-3, -0x1.a6d1f2a204a8cp-6,
    0x1.f9d38a3763cc3p-10, -0x1.b6e24f44b128fp-14, 0x1.20c62c2f2d7f5p-18,
    -0x1.2a0c591af8314p-23, 0x1.ef6e308d6d1c4p-29, -0x1.52ae4120fde27p-34,
    0x1.838d8f4321800p-40, -0x1.789d662bb5482p-46,
};
static const double lg_cospi_b_lo_[14] = {
    0x0.0p+0, -0x1.692b71366cc04p-52, -0x1.32b33f87fc145p-52,
    0x1.d582920937625p-59, -0x1.7362f495c096dp-60, 0x1.5961232276df6p-60,
    -0x1.c8a14c8bd6bc5p-64, -0x1.6de1e0a0c23b9p-69, -0x1.5a3cd1a11c7a2p-72,
    -0x1.215803afbd5f8p-77, -0x1.c5f7779fbdd48p-83, 0x1.76dd247cd9002p-88,
    -0x1.453680e7f5659p-96, -0x1.01d70ae199b04p-104,
};

/* Evaluate sum_k c_k * u^k (c in dd arrays, k=0..7) by Horner in u (dd). */
/* sin(pi f)/(pi f) = G(f^2) = sum_k c_k f^(2k), c_k=(-1)^k pi^(2k)/(2k+1)!;
 * |f|<=0.25 so f^2<=0.0625, truncation < 1.2e-22. */
static const double lg_sinc_hi_[10] = {
    1.0,
    -1.6449340668482264,
    0.8117424252833536,
    -0.19075182412208422,
    0.0261478478176548,
    -0.0023460810354558235,
    0.000148428793031071,
    -6.975873661656381e-06,
    2.5312174041370274e-07,
    -7.304711822217775e-09,
};
static const double lg_sinc_lo_[10] = {
    0.0,
    -3.040672350398476e-17,
    3.561384032141524e-17,
    4.4195856292634144e-18,
    6.311763718038651e-19,
    -1.6959772863819877e-19,
    7.156938521930286e-21,
    2.3386829645434924e-22,
    2.3636074197084703e-23,
    1.7231504593537484e-25,
};

static double lg_poly_(const double *chi, const double *clo, int n, double uhi, double ulo, double *rlo)
{
    double phi = chi[n - 1], plo = clo[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        double t;
        phi = gamma_mul_dd_(phi, plo, uhi, ulo, &t);
        plo = t;
        phi = gamma_twosum_(phi, chi[i], &t);
        plo += t + clo[i];
    }
    *rlo = plo;
    return phi;
}

static double lg_sinpi_dd_(double t, double *rlo)
{
    double a = fabs(t);
    double r, rl;
    if (a <= 0.25) {
        double u2lo, u2 = gamma_twoprod_(a, a, &u2lo);
        double slo, s = lg_poly_(lg_sinpi_a_hi_, lg_sinpi_a_lo_, 8, u2, u2lo, &slo);
        r = gamma_mul_dd_(s, slo, a, 0.0, &rl);
    } else {
        double vlo, v = gamma_twosum_(0.5, -a, &vlo);
        double v2lo, v2 = gamma_mul_dd_(v, vlo, v, vlo, &v2lo);
        r = lg_poly_(lg_cospi_b_hi_, lg_cospi_b_lo_, 14, v2, v2lo, &rl);
    }
    if (t < 0.0) { r = -r; rl = -rl; }
    *rlo = rl;
    return r;
}

/* ---- double-double natural log of a positive double (x_hi + x_lo) ---- *
 * ln x = e*ln2 + 2*atanh(f), f = (m-1)/(m+1), m = mantissa in [sqrt(2)/2, sqrt(2)),
 * so |f| <= 0.1716.  atanh(f) = f * (1 + w/3 + w^2/5 + ...), w = f^2 <= 0.0295;
 * the series is carried to w^16 (f^33), truncation < 1e-26, which is below the
 * double-double round-off, so the (hi,lo) pair is full precision. */
static double lg_log_dd_(double x_hi, double x_lo, double *rlo)
{
    int e;
    double m = frexp(x_hi, &e);
    if (m < 0.7071067811865476) {
        m *= 2.0;
        e--;
    }
    /* f = (m-1)/(m+1) in double-double. */
    double numlo, num = gamma_twosum_(m, -1.0, &numlo);
    double denlo, den = gamma_twosum_(m, 1.0, &denlo);
    double flo, f = lg_div_dd_(num, numlo, den, denlo, &flo);
    /* w = f^2 (double-double). */
    double wlo, w = gamma_mul_dd_(f, flo, f, flo, &wlo);
    /* bracket B(w) = 1 + w/3 + w^2/5 + ... via dd Horner, coefficients 1/(2k+1). */
    static const double cinv[16] = {
        1.0 / 33.0, 1.0 / 31.0, 1.0 / 29.0, 1.0 / 27.0,
        1.0 / 25.0, 1.0 / 23.0, 1.0 / 21.0, 1.0 / 19.0,
        1.0 / 17.0, 1.0 / 15.0, 1.0 / 13.0, 1.0 / 11.0,
        1.0 /  9.0, 1.0 /  7.0, 1.0 /  5.0, 1.0 /  3.0,
    };
    double phi = cinv[0], plo = 0.0;
    for (int i = 1; i < 16; i++) {
        double t;
        phi = gamma_mul_dd_(phi, plo, w, wlo, &t);
        plo = t;
        phi = gamma_twosum_(phi, cinv[i], &t);
        plo += t;
    }
    /* B = 1 + w * phi */
    double bplo, bp = gamma_mul_dd_(phi, plo, w, wlo, &bplo);
    bp = gamma_twosum_(bp, 1.0, &bplo);
    /* atanh(f) = f * B */
    double ahlo, ah = gamma_mul_dd_(f, flo, bp, bplo, &ahlo);
    /* ln x_hi = e*ln2 + 2*atanh(f) */
    double t2lo, t2 = gamma_mul_dd_(ah, ahlo, 2.0, 0.0, &t2lo);
    double elo, eln = gamma_mul_dd_((double)e, 0.0, LG_LN2_HI, LG_LN2_LO, &elo);
    double r = gamma_twosum_(eln, t2, rlo);
    *rlo += elo + t2lo;
    /* ln(x_hi + x_lo) = ln(x_hi) + log1p(x_lo/x_hi); linear term suffices. */
    if (x_lo != 0.0) {
        double c = x_lo / x_hi;
        double t;
        r = gamma_twosum_(r, c, &t);
        *rlo += t;
    }
    return r;
}


/* ln|sin(pi f)| for |f| <= 0.5 as a double-double, cancellation-free.
 * For |f| <= 0.25: sin(pi f) = (pi f) * G(f^2), so
 *   ln|sin(pi f)| = ln|f| + ln(pi) + ln G(f^2),
 * where G is bounded in [2/pi, 1] (no cancellation) and ln|f| is taken by the
 * full-precision dd log.  For 0.25 < |f| <= 0.5, sin(pi f) is bounded away from
 * zero, so ln|sin| from lg_sinpi_dd_ + lg_log_dd_ is already accurate. */
static double lg_logsin_dd_(double f, double *rlo)
{
    double a = fabs(f);
    if (a <= 0.25) {
        double v2lo, v2 = gamma_twoprod_(a, a, &v2lo);          /* f^2 dd */
        /* G(f^2) via dd Horner over lg_sinc_. */
        double ghi = lg_sinc_hi_[9], glo = lg_sinc_lo_[9];
        for (int i = 8; i >= 0; i--) {
            double t;
            ghi = gamma_mul_dd_(ghi, glo, v2, v2lo, &t);
            glo = t;
            ghi = gamma_twosum_(ghi, lg_sinc_hi_[i], &t);
            glo += t + lg_sinc_lo_[i];
        }
        double lglo, lg = lg_log_dd_(ghi, glo, &lglo);          /* ln G */
        double lflo, lf = lg_log_dd_(a, 0.0, &lflo);            /* ln|f| */
        double pplo, pp = lg_log_dd_(LG_PI_HI, LG_PI_LO, &pplo);/* ln pi */
        double t, hi = gamma_twosum_(lf, pp, rlo);
        *rlo += lflo + pplo;
        hi = gamma_twosum_(hi, lg, &t);
        *rlo += t + lglo;
        return hi;
    }
    double splo, sp = lg_sinpi_dd_(f, &splo);
    if (sp < 0.0) { sp = -sp; splo = -splo; }
    return lg_log_dd_(sp, splo, rlo);
}

#endif

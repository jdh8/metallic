#include "kernel/gamma.h"
#include <math.h>

/* Fast minimax for Gamma(2.875 + d), d in [-0.5, 0.5].
 * Relative error is bounded near 2^-42, enough for the Ziv gate. */
static const double tgamma_poly_f64_[12] = {
    1.7877108988966335,
    1.5591939012079723,
    1.0510493267695413,
    0.47065801828715076,
    0.18881863420183825,
    0.05883154870082071,
    0.017826013745994518,
    0.004228753264700664,
    0.001097379867515545,
    0.00019460263214349485,
    5.3863392739342684e-5,
    4.788385422314536e-6,
};

static double tgamma_poly_eval_(double x, const double *c, int n)
{
    double y = c[n - 1];

    for (int i = n - 2; i >= 0; --i)
        y = y * x + c[i];

    return y;
}

/* Gamma(2.875 + d) as a double-double for d in [-0.5, 0.5].
 *
 * Degree-18 minimax from mpmath.chebyfit(gamma(2.875+d), [-0.5,0.5], 19) at
 * prec=260; relative approximation error ~2^-68.  The six leading coefficients
 * are carried as double-double (their f64 rounding would otherwise enter at
 * ~2^-53); the tail c6..c18 is summed in plain f64 (its rounding enters only at
 * d^6, below 2^-62) and folded into the dd Horner pass.  This is the accurate
 * analogue of tgamma_poly_f64_ used by the fast path. */
static const double tgamma_dd_hi_[6] = {
    0x1.c9a76be577123p+0, 0x1.8f2754ddcf90cp+0, 0x1.0d1191949418ap+0,
    0x1.e1f42cf0ae65ep-2, 0x1.82b358a3ae616p-3, 0x1.e1f2b30cb100bp-5,
};
static const double tgamma_dd_lo_[6] = {
    -0x1.58baa4910955dp-55, -0x1.f91aaf754e977p-58, -0x1.75f1d7b24101cp-54,
     0x1.4eccfb1f0e199p-60, -0x1.d86fa29f03bacp-59, -0x1.9e340af621b03p-65,
};
static const double tgamma_tail_[13] = {
     0x1.240f6d3d63a06p-6,  0x1.1522ca0d963cfp-8,  0x1.1fd00825c3345p-10,
     0x1.980885bff4e6bp-13, 0x1.b3f446a8d8113p-15, 0x1.49e2edc896c01p-18,
     0x1.48150757752ffp-19, -0x1.41d67b864cdf7p-23, 0x1.6269864c4a0c4p-23,
    -0x1.57bf34cf1bfe7p-25, 0x1.1a074caa43f5ap-26, -0x1.b0c82a252e56ep-28,
     0x1.33ad3a2fc226ep-29,
};

/* Renormalizing double-double sum (a_hi+a_lo)+(b_hi+b_lo) -> (hi, *lo). */
static double tg_add_dd_(double a_hi, double a_lo, double b_hi, double b_lo, double lo[static 1])
{
    double e;
    double s = gamma_twosum_(a_hi, b_hi, &e);
    e += a_lo + b_lo;
    double hi = s + e;
    *lo = s - hi + e;
    return hi;
}

/* Gamma(2.875 + d) in double-double via dd Horner over the six leading
 * coefficients, with the f64 tail folded into the lowest term. */
static double tgamma_poly_dd_(double d, double lo[static 1])
{
    double tail = tgamma_poly_eval_(d, tgamma_tail_, 13);

    /* acc = c5 + d*tail */
    double plo, phi = gamma_twoprod_(d, tail, &plo);
    double acc_lo, acc_hi = tg_add_dd_(tgamma_dd_hi_[5], tgamma_dd_lo_[5], phi, plo, &acc_lo);

    for (int k = 4; k >= 0; --k) {
        double t;
        acc_hi = gamma_mul_dd_(acc_hi, acc_lo, d, 0.0, &t);       /* acc * d */
        acc_hi = tg_add_dd_(acc_hi, t, tgamma_dd_hi_[k], tgamma_dd_lo_[k], &acc_lo);
    }

    *lo = acc_lo;
    return acc_hi;
}

/* Accurate double-double fallback for the hard-to-round cases that survive the
 * fast Ziv gate.  Mirrors tgamma_f64_: reduce x into the minimax interval around
 * 2.875, evaluate Gamma there in double-double, then walk the recurrence
 *   Gamma(x) = Gamma(x-steps) * prod_{j=1..steps}(x-j)    (x above the interval)
 *   Gamma(x) = Gamma(x+steps) / prod_{j=0..steps-1}(x+j)  (x below)
 * carrying everything in double-double.  Negative x flows through the same
 * recurrence -- the product runs through negative factors and supplies the
 * sign, so no reflection is needed (matching the fast path, against which the
 * Ziv gate compares).  The earlier Lanczos path was capped at ~2^-41 by the
 * float-accuracy log2f_ it relied on, so it could not round these correctly. */
static float tgammaf_dd_(float z)
{
    double x = (double)z;
    double m = x - 2.875;
    double i = nearbyint(m);
    double lo, hi = tgamma_poly_dd_(m - i, &lo);
    int steps = (int)fabs(i);

    if (i > 0.0) {
        double factor = x;
        for (int k = 0; k < steps; ++k) {
            factor -= 1.0;
            hi = gamma_mul_dd_(hi, lo, factor, 0.0, &lo);
        }
    } else if (i < 0.0) {
        double prod_lo = 0.0, prod_hi = x, factor = x;
        for (int k = 1; k < steps; ++k) {
            factor += 1.0;
            prod_hi = gamma_mul_dd_(prod_hi, prod_lo, factor, 0.0, &prod_lo);
        }
        double rlo, rhi = gamma_div_dd_(1.0, prod_hi, prod_lo, &rlo);
        hi = gamma_mul_dd_(hi, lo, rhi, rlo, &lo);
    }

    return gamma_to_float_odd_(hi, lo);
}

/* Fast f64 approximation over the recurrence range with an absolute bound. */
static double tgamma_f64_(double x, double err[static 1])
{
    double m = x - 2.875;
    double i = nearbyint(m);
    double value = tgamma_poly_eval_(m - i, tgamma_poly_f64_, 12);
    int steps = (int)fabs(i);

    if (i > 0.0) {
        double factor = x;

        for (int k = 0; k < steps; ++k) {
            factor -= 1.0;
            value *= factor;
        }
    } else if (i < 0.0) {
        double product = x;
        double factor = x;

        for (int k = 1; k < steps; ++k) {
            factor += 1.0;
            product *= factor;
        }

        value /= product;
    }

    *err = ldexp(fabs(value), -37);
    return value;
}

float tgammaf(float z)
{
    if (isnan(z))
        return z;

    if (z == 0)
        return copysignf(INFINITY, z);

    if (z == INFINITY)
        return INFINITY;

    /* Pole neighborhood: Gamma(z) = 1/z - gamma + O(z). */
    if (fabsf(z) < 0x1p-12f) {
        const double c[] = {
            -0.5772156649015329,
             0.9890559953279726,
            -0.9074790760808863,
             0.9817280868344002,
        };

        double x = (double)z;
        double correction = tgamma_poly_eval_(x, c, 4);
        double q_lo;
        double q_hi = gamma_div_dd_(1.0, x, 0.0, &q_lo);
        double t;
        double y_hi = gamma_twosum_(q_hi, correction, &t);
        double y_lo = q_lo + t;

        return gamma_to_float_odd_(y_hi, y_lo);
    }

    if (z >= 35.04010009765625f)
        return INFINITY;

    if (z < 0.0f && truncf(z) == z)
        return NAN;

    if (z < -42.0f)
        return (((long long)floor((double)z)) & 1LL) == 0 ? 0.0f : -0.0f;

    double x = (double)z;
    double err;
    double f = tgamma_f64_(x, &err);
    float lo = (float)(f - err);
    float hi = (float)(f + err);

    if (lo == hi)
        return lo;

    return tgammaf_dd_(z);
}

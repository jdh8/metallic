#include "kernel/gamma.h"
#include "finite/log2f.h"
#include "exp2f.h"
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

/* Lanczos partial-fraction sum carried in double-double.
 *
 *   S(z) = p0 + p1/(z+1) + p2/(z+2) + p3/(z+3) + p4/(z+4)
 *
 * The p1 and p2 terms are large and of opposite sign, so they partially
 * cancel; accumulating with two-sum keeps the low part the final cast needs.
 * Coefficients are the float Lanczos set from kernel/lanczos.h (array p,
 * g = 4.3644...); they are reproduced here so each quotient can also yield its
 * residual for the double-double carry. */
static double lanczos_series_dd_(double z, double lo[static 1])
{
    const double p[] = {
        2.5066282972608683788,
        93.724008703110370647,
       -85.030278048785721233,
        15.313090747320992728,
       -0.2376573970640603182
    };

    double s = p[0];
    double e = 0;

    for (int k = 1; k <= 4; ++k) {
        double d = z + k;
        double t = p[k] / d;

        /* residual of the quotient: t_lo = (p[k] - t*d) / d, with t*d fused. */
        double t_lo = fma(-t, d, p[k]) / d;

        double a;
        s = gamma_twosum_(s, t, &a);
        e += a + t_lo;
    }

    double hi = s + e;
    *lo = s - hi + e;
    return hi;
}

/* Gamma(1+z) in double-double for z >= -0.5.
 *
 *   Gamma(1+z) = 2^E * S(z),   E = (0.5+z)*log2(base) - log2e*base,
 *   base = z + g + 0.5.
 *
 * The two terms of E are large and nearly cancel, so E is formed in
 * double-double; 2^E is then exp2(E_hi) corrected by the first-order term
 * ln2*E_lo, and multiplied by the double-double Lanczos sum. */
static double gamma1p_dd_(double z, double lo[static 1])
{
    const double log2e = 1.442695040888963407;
    const double ln2 = 0.69314718055994530942;
    const double lanczos_g = 4.3644453082153116114;

    double base = lanczos_g + 0.5 + z;
    double l2 = log2f_(base);

    double a_lo;
    double a_hi = gamma_twoprod_(0.5 + z, l2, &a_lo);

    double b_lo;
    double b_hi = gamma_twoprod_(log2e, base, &b_lo);

    double s_lo;
    double e_hi = gamma_twosum_(a_hi, -b_hi, &s_lo);
    double e_lo = s_lo + (a_lo - b_lo);

    /* 2^(e_hi + e_lo) ~= exp2(e_hi) * (1 + ln2*e_lo). */
    double g = exp2f_(e_hi);
    double g_lo = g * (ln2 * e_lo);

    double s_low;
    double s_hi = lanczos_series_dd_(z, &s_low);

    return gamma_mul_dd_(g, g_lo, s_hi, s_low, lo);
}

/* Legacy fully double-double path. Kept as the accurate fallback for the rare
 * hard-to-round cases that survive the fast Ziv gate. */
static float tgammaf_dd_(float z)
{
    const double pi_hi = 3.14159265358979312;
    const double pi_lo = 1.2246467991473532e-16;

    if (z == 0)
        return copysignf(INFINITY, z);

    if (isnan(z))
        return z;

    if (z == INFINITY)
        return INFINITY;

    if (z < 0.5f) {
        if (rintf(z) == z)
            return NAN;

        double sp_lo;
        double sp = gamma_sinpi_dd_(z, &sp_lo);

        if (z < -170.0f)
            return copysignf(0.0f, sp);

        double g_lo;
        double g_hi = gamma1p_dd_(-z, &g_lo);

        double d_lo;
        double d_hi = gamma_mul_dd_(sp, sp_lo, g_hi, g_lo, &d_lo);

        double q_lo;
        double q_hi = gamma_div_dd_(pi_hi, d_hi, d_lo, &q_lo);

        q_lo += pi_lo / d_hi;

        return gamma_to_float_odd_(q_hi, q_lo);
    }

    if (z >= 36.0f)
        return INFINITY;

    double lo;
    double hi = gamma1p_dd_(z - 1.0, &lo);

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

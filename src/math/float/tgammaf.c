#include "kernel/gamma.h"
#include "kernel/lanczos.h"
#include "finite/log2f.h"
#include "finite/sinpif.h"
#include "exp2f.h"
#include <math.h>

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

    double base = lanczos_g_ + 0.5 + z;
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

float tgammaf(float z)
{
    const double pi_hi = 3.14159265358979312;
    const double pi_lo = 1.2246467991473532e-16;

    if (z == 0)
        return copysignf(INFINITY, z);

    if (z == INFINITY)
        return INFINITY;

    if (z < 0.5f) {
        if (rintf(z) == z)
            return NAN;

        /* Reflection: Gamma(z) = pi / (sin(pi z) * Gamma(1-z)).  Carry the
         * denominator and the division in double-double, fold in the low part
         * of pi, then round the pair to the nearest float. */
        double sp = sinpif_(z);

        double g_lo;
        double g_hi = gamma1p_dd_(-z, &g_lo);

        double d_lo;
        double d_hi = gamma_mul_dd_(sp, 0.0, g_hi, g_lo, &d_lo);

        double q_lo;
        double q_hi = gamma_div_dd_(pi_hi, d_hi, d_lo, &q_lo);

        q_lo += pi_lo / d_hi;

        return gamma_to_float_odd_(q_hi, q_lo);
    }

    double lo;
    double hi = gamma1p_dd_(z - 1.0, &lo);

    return gamma_to_float_odd_(hi, lo);
}

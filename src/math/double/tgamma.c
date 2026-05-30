/* tgamma(x) = Γ(x)
 *
 * Method: Lanczos approximation (g=7, n=9; Boost lanczos13m53 coefficients).
 *
 * For x >= 0.5:
 *   Γ(x) = exp(lgamma_lanczos_(x-1))
 *   The restructured formula avoids intermediate overflow for x up to ~171.6.
 *
 * For x in [0.5, 1.5]: use exp(kernel_lgamma1p_(x-1)) for near-cancellation accuracy.
 * For x in [1.5, 4.5]: use recurrence to reduce to [0.5,1.5] and multiply.
 *
 * For x < 0.5 (reflection):
 *   Γ(x) = π / (sin(πx) * Γ(1-x))
 *   For large |x| where Γ(1-x) overflows, use exp form instead.
 *
 * Special cases (C11/Annex F):
 *   tgamma(+∞) = +∞
 *   tgamma(-∞) = NaN
 *   tgamma(±0) = ±∞ (pole)
 *   tgamma(negative integer) = NaN (pole)
 *   tgamma(x) = +∞ for x > 171.6247...
 */

#include "kernel/lanczos.h"
#include "kernel/sincos.h"
#include "../reinterpret.h"
#include <math.h>

/* sin(π*x) for arbitrary finite double x. */
static double sinpi_(double x)
{
    double r = x - 2.0 * rint(0.5 * x);
    double q_f = rint(2.0 * r);
    int q = (int)q_f & 3;
    double t = r - 0.5 * q_f;
    const double pi = 3.14159265358979323846;
    double a = pi * t;
    switch (q) {
        case 0: return  kernel_sin_(a, 0.0);
        case 1: return  kernel_cos_(a, 0.0);
        case 2: return -kernel_sin_(a, 0.0);
        case 3: return -kernel_cos_(a, 0.0);
    }
    __builtin_unreachable();
}

/* half_ln2pi = 0.5 * ln(2π) */
static const double half_ln2pi_ = 0.9189385332046727417803;

/* lgamma_lanczos_(z) = lgamma(z+1) via Lanczos (restructured to reduce overflow).
 * Valid for z >= -0.5 (x = z+1 >= 0.5). */
static double lgamma_lanczos_(double z)
{
    double base = lanczos_g_ + 0.5 + z;
    return (0.5 + z) * (log(base) - 1.0) - lanczos_g_ + half_ln2pi_ + log(lanczos_sum_(z));
}

/* 52-term Maclaurin series for lgamma(1+t)/t, accurate < 0.25 ulp for |t| <= 0.5. */
static double kernel_lgamma1p_(double t)
{
    static const double c[] = {
        -0x1.2788cfc6fb619p-1,   0x1.a51a6625307d3p-1,  -0x1.9a4d55beab2d7p-2,
         0x1.151322ac7d848p-2,  -0x1.a8b9c17aa6149p-3,   0x1.5b40cb100c306p-3,
        -0x1.2703a1dcea3aep-3,   0x1.010b36af86397p-3,  -0x1.c806706d57db4p-4,
         0x1.9a01e385d5f8fp-4,  -0x1.748c33114c6d6p-4,   0x1.556ad63243bc4p-4,
        -0x1.3b1d971fc5985p-4,   0x1.2496df8320c5fp-4,  -0x1.11133476e7fe0p-4,
         0x1.00010064cdeb2p-4,  -0x1.e1e2d311e8abdp-5,   0x1.c71ce3a20b419p-5,
        -0x1.af28a1b5688a0p-5,   0x1.9999b3352d5bap-5,  -0x1.86186db77bfbfp-5,
         0x1.745d1d1778df9p-5,  -0x1.642c88591b66dp-5,   0x1.555556aaafdcdp-5,
        -0x1.47ae151eb9fb7p-5,   0x1.3b13b189d925ep-5,  -0x1.2f684c00002bcp-5,
         0x1.24924936db7bcp-5,  -0x1.1a7b961a7b9aap-5,   0x1.111111155556dp-5,
        -0x1.08421086318cep-5,   0x1.0000000100002p-5,  -0x1.f07c1f08ba2eap-6,
         0x1.e1e1e1e25a5a6p-6,  -0x1.d41d41d457c58p-6,   0x1.c71c71c738e39p-6,
        -0x1.bacf914c29837p-6,   0x1.af286bca21af3p-6,  -0x1.a41a41a41d89ep-6,
         0x1.999999999b333p-6,  -0x1.8f9c18f9c2577p-6,   0x1.8618618618c31p-6,
        -0x1.7d05f417d08eep-6,   0x1.745d1745d18bap-6,  -0x1.6c16c16c16ccdp-6,
         0x1.642c8590b21bdp-6,  -0x1.5c9882b931083p-6,   0x1.555555555556bp-6,
        -0x1.4e5e0a72f0544p-6,   0x1.47ae147ae1480p-6,  -0x1.4141414141417p-6,
         0x1.3b13b13b13b15p-6,
    };
    double s = c[51];
    for (int i = 50; i >= 0; --i)
        s = s * t + c[i];
    return s * t;
}

/* lgamma_pos_(x) for x >= 0.5: uses polynomial for x in [0.5,4.5] and Lanczos for x>4.5. */
static double lgamma_pos_(double x)
{
    if (x <= 1.5)
        return kernel_lgamma1p_(x - 1.0);
    if (x <= 2.5)
        return kernel_lgamma1p_(x - 2.0) + log1p(x - 2.0);
    if (x <= 3.5)
        return kernel_lgamma1p_(x - 3.0) + log(x - 2.0) + log1p(x - 2.0);
    if (x <= 4.5)
        return kernel_lgamma1p_(x - 4.0) + log(x - 3.0) + log(x - 2.0) + log1p(x - 2.0);
    if (x > 0x1p1014)
        return x * (log(x) - 1.0) - 0.5 * log(x) + half_ln2pi_ + 1.0 / (12.0 * x);
    return lgamma_lanczos_(x - 1.0);
}

/* tgamma(x) for x >= 0.5 via exp(lgamma_pos_(x)).
 * For x in [0.5, 4.5]: use recurrence to avoid exp(large) precision loss. */
static double gamma_pos_(double x)
{
    /* For small x, the polynomial is accurate enough to just exp it. */
    if (x <= 1.5)
        return exp(kernel_lgamma1p_(x - 1.0));

    /* For x in (1.5, 4.5]: use recurrence Γ(x) = (x-1)*Γ(x-1) until x <= 1.5.
     * The recurrence introduces at most a few ulp per step (3 steps max). */
    if (x <= 4.5) {
        double t = x;
        double result = 1.0;
        while (t > 1.5) {
            t -= 1.0;
            result *= t;
        }
        return result * exp(kernel_lgamma1p_(t - 1.0));
    }

    /* Large x: use lgamma via Lanczos then exp.
     * The Lanczos formula has ~5 ulp in lgamma, which translates to
     * ~5*lgamma(x) ulp in tgamma for large x (fundamental limitation of g=7 n=9). */
    return exp(lgamma_pos_(x));
}

double tgamma(double x)
{
    const double pi = 3.14159265358979323846;

    if (isnan(x))
        return x;

    /* tgamma(+inf) = +inf, tgamma(-inf) = NaN */
    if (isinf(x))
        return x > 0 ? INFINITY : NAN;

    /* tgamma(±0) = ±inf (pole) */
    if (x == 0.0)
        return copysign(INFINITY, x);

    /* Negative integers: pole, return NaN */
    if (x < 0.0 && (x <= -0x1p52 || x == (double)(long long)x))
        return NAN;

    /* Overflow: Γ(x) > DBL_MAX for x beyond the point where lgamma(x) > log(DBL_MAX).
     * The threshold is x ≈ 171.6243769... Use a hex constant just above the last
     * representable finite x: 0x1.573fae561f648p+7 ≈ 171.6243769... */
    if (x >= 0x1.573fae561f648p+7)
        return INFINITY;

    if (x >= 0.5)
        return gamma_pos_(x);

    /* Reflection: Γ(x) = π / (sin(πx) * Γ(1-x)) */
    double sinpx = sinpi_(x);

    /* For x <= -170.5, 1-x >= 171.5, and Γ(1-x) overflows.
     * Use log-space: Γ(x) = exp(log(π) - log|sin(πx)| - lgamma_pos_(1-x)) * sign */
    if (x <= -170.5) {
        double lgtx = log(pi) - log(fabs(sinpx)) - lgamma_pos_(1.0 - x);
        return copysign(exp(lgtx), sinpx);
    }

    return pi / (sinpx * gamma_pos_(1.0 - x));
}

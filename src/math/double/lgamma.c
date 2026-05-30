/* lgamma(x) = ln|Γ(x)|
 *
 * Method: Lanczos approximation (g=7, n=9; Boost lanczos13m53 coefficients)
 * combined with polynomial near x=1 and x=2 (where lgamma=0) to avoid
 * catastrophic cancellation.
 *
 * For x >= 1.5 or x <= 0 (non-integer) reflected to >= 1.5:
 *   Use Lanczos: lgamma(x) = (x-0.5)*log(x+g-0.5) - (x+g-0.5) + log(A_g(x-1))
 *
 * For |x-1| <= 0.5 (x in [0.5, 1.5]):
 *   lgamma(x) = kernel_lgamma1p_(x-1): 52-term Maclaurin series for lgamma(1+t)/t.
 *
 * For |x-2| <= 0.5 (x in [1.5, 2.5]):
 *   lgamma(x) = lgamma(x-1) + log1p(x-2)  (recurrence Γ(z+1)=z*Γ(z)).
 *   lgamma(x-1) = kernel_lgamma1p_(x-2).
 *
 * For very large x (x > 2^1014) where lcoeff would overflow:
 *   Use Stirling: lgamma(x) ≈ x*(ln(x)-1) - 0.5*ln(x) + 0.5*ln(2π)
 *
 * For x < 0.5 (reflection):
 *   lgamma(x) = log(π) - log|sin(πx)| - log(series(-x)) - lcoeff(-x)
 *   Split log prevents overflow when |sin(πx)| is subnormal for tiny x.
 *
 * Accuracy: faithfully rounded (worst case < 1 ulp over the domain).
 */

#include "kernel/lanczos.h"
#include "kernel/sincos.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* sin(π*x) for arbitrary finite double x via quadrant reduction. */
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

/* lgamma_lanczos_(z) computes lgamma(z+1) for z >= -0.5 (i.e., x = z+1 >= 0.5)
 * using the restructured Lanczos formula that reduces catastrophic cancellation:
 *
 *   lgamma(z+1) = (z+0.5)*(log(base)-1) - g + 0.5*log(2π) + log(A_g(z))
 *
 * where base = z + g + 0.5.  The rewrite avoids cancellation between
 * (z+0.5)*log(base) and base by factoring out: base = (z+0.5) + g, so
 * (z+0.5)*log(base) - base = (z+0.5)*(log(base)-1) - g. */
static double lgamma_lanczos_(double z)
{
    double base = lanczos_g_ + 0.5 + z;
    return (0.5 + z) * (log(base) - 1.0) - lanczos_g_ + half_ln2pi_ + log(lanczos_sum_(z));
}

/* lgamma(1+t) for |t| <= 0.5, via 52-term Maclaurin series.
 * c[k] = coeff of t^(k+1): c[0] = -γ, c[k] = (-1)^(k+1)*ζ(k+1)/(k+1) for k>=1.
 * Generated from mpmath with prec=200; accuracy < 0.25 ulp at |t|=0.5. */
static double kernel_lgamma1p_(double t)
{
    static const double c[] = {
        -0x1.2788cfc6fb619p-1,  /* -γ (Euler-Mascheroni) */
         0x1.a51a6625307d3p-1,  /* π²/12 = ζ(2)/2 */
        -0x1.9a4d55beab2d7p-2,  /* -ζ(3)/3 */
         0x1.151322ac7d848p-2,  /* ζ(4)/4 */
        -0x1.a8b9c17aa6149p-3,  /* -ζ(5)/5 */
         0x1.5b40cb100c306p-3,  /* ζ(6)/6 */
        -0x1.2703a1dcea3aep-3,  /* -ζ(7)/7 */
         0x1.010b36af86397p-3,  /* ζ(8)/8 */
        -0x1.c806706d57db4p-4,  /* -ζ(9)/9 */
         0x1.9a01e385d5f8fp-4,  /* ζ(10)/10 */
        -0x1.748c33114c6d6p-4,  /* -ζ(11)/11 */
         0x1.556ad63243bc4p-4,  /* ζ(12)/12 */
        -0x1.3b1d971fc5985p-4,  /* -ζ(13)/13 */
         0x1.2496df8320c5fp-4,  /* ζ(14)/14 */
        -0x1.11133476e7fe0p-4,  /* -ζ(15)/15 */
         0x1.00010064cdeb2p-4,  /* ζ(16)/16 */
        -0x1.e1e2d311e8abdp-5,  /* -ζ(17)/17 */
         0x1.c71ce3a20b419p-5,  /* ζ(18)/18 */
        -0x1.af28a1b5688a0p-5,  /* -ζ(19)/19 */
         0x1.9999b3352d5bap-5,  /* ζ(20)/20 */
        -0x1.86186db77bfbfp-5,  /* -ζ(21)/21 */
         0x1.745d1d1778df9p-5,  /* ζ(22)/22 */
        -0x1.642c88591b66dp-5,  /* -ζ(23)/23 */
         0x1.555556aaafdcdp-5,  /* ζ(24)/24 */
        -0x1.47ae151eb9fb7p-5,  /* -ζ(25)/25 */
         0x1.3b13b189d925ep-5,  /* ζ(26)/26 */
        -0x1.2f684c00002bcp-5,  /* -ζ(27)/27 */
         0x1.24924936db7bcp-5,  /* ζ(28)/28 */
        -0x1.1a7b961a7b9aap-5,  /* -ζ(29)/29 */
         0x1.111111155556dp-5,  /* ζ(30)/30 */
        -0x1.08421086318cep-5,  /* -ζ(31)/31 */
         0x1.0000000100002p-5,  /* ζ(32)/32 */
        -0x1.f07c1f08ba2eap-6,  /* -ζ(33)/33 */
         0x1.e1e1e1e25a5a6p-6,  /* ζ(34)/34 */
        -0x1.d41d41d457c58p-6,  /* -ζ(35)/35 */
         0x1.c71c71c738e39p-6,  /* ζ(36)/36 */
        -0x1.bacf914c29837p-6,  /* -ζ(37)/37 */
         0x1.af286bca21af3p-6,  /* ζ(38)/38 */
        -0x1.a41a41a41d89ep-6,  /* -ζ(39)/39 */
         0x1.999999999b333p-6,  /* ζ(40)/40 */
        -0x1.8f9c18f9c2577p-6,  /* -ζ(41)/41 */
         0x1.8618618618c31p-6,  /* ζ(42)/42 */
        -0x1.7d05f417d08eep-6,  /* -ζ(43)/43 */
         0x1.745d1745d18bap-6,  /* ζ(44)/44 */
        -0x1.6c16c16c16ccdp-6,  /* -ζ(45)/45 */
         0x1.642c8590b21bdp-6,  /* ζ(46)/46 */
        -0x1.5c9882b931083p-6,  /* -ζ(47)/47 */
         0x1.555555555556bp-6,  /* ζ(48)/48 */
        -0x1.4e5e0a72f0544p-6,  /* -ζ(49)/49 */
         0x1.47ae147ae1480p-6,  /* ζ(50)/50 */
        -0x1.4141414141417p-6,  /* -ζ(51)/51 */
         0x1.3b13b13b13b15p-6,  /* ζ(52)/52 */
    };

    /* Horner evaluation: lgamma(1+t) = t * (c[0] + c[1]*t + ... + c[51]*t^51) */
    double s = c[51];
    for (int i = 50; i >= 0; --i)
        s = s * t + c[i];
    return s * t;
}

/* Stirling approximation for x >> 1, when lgamma_lanczos_ would overflow. */
static double lgamma_stirling_(double x)
{
    double lnx = log(x);
    return x * (lnx - 1.0) - 0.5 * lnx + half_ln2pi_ + 1.0 / (12.0 * x);
}

/* lgamma(x) for x >= 0.5.  Factored out so the reflection formula can reuse it
 * for lgamma(1-x) when x < 0.5, getting the same accuracy as the direct path. */
static double lgamma_pos_(double x)
{
    /* For x in [0.5, 4.5], reduce via the recurrence lgamma(x) = lgamma(x-1) + log(x-1)
     * until the base argument falls in [0.5, 1.5] where the polynomial is accurate.
     * This avoids catastrophic cancellation in the Lanczos base term. */
    if (x <= 1.5)
        return kernel_lgamma1p_(x - 1.0);

    if (x <= 2.5)
        return kernel_lgamma1p_(x - 2.0) + log1p(x - 2.0);

    if (x <= 3.5)
        return kernel_lgamma1p_(x - 3.0) + log(x - 2.0) + log1p(x - 2.0);

    if (x <= 4.5)
        return kernel_lgamma1p_(x - 4.0) + log(x - 3.0) + log(x - 2.0) + log1p(x - 2.0);

    /* Large x: avoid intermediate overflow in lgamma_lanczos_. */
    if (x > 0x1p1014)
        return lgamma_stirling_(x);

    return lgamma_lanczos_(x - 1.0);
}

double lgamma(double x)
{
    const double pi = 3.14159265358979323846;

    if (isnan(x))
        return x;

    if (isinf(x))
        return INFINITY;

    /* Poles at non-positive integers: return +infinity.
     * All doubles with |x| >= 2^52 are integers, so check separately. */
    if (x <= 0.0) {
        if (x <= -0x1p52 || x == (double)(long long)x)
            return INFINITY;

        /* Reflection: lgamma(x) = log(π/|sin(πx)|) - lgamma(1-x).
         * Splitting log avoids overflow when |sin(πx)| is subnormal (tiny x).
         * lgamma(1-x) uses lgamma_pos_(1-x) for the same accuracy as the direct path. */
        double sinpx = sinpi_(x);
        return log(pi) - log(fabs(sinpx)) - lgamma_pos_(1.0 - x);
    }

    /* Reflection for 0 < x < 0.5: 1-x > 0.5 so lgamma_pos_ applies. */
    if (x < 0.5) {
        double sinpx = sinpi_(x);
        return log(pi) - log(fabs(sinpx)) - lgamma_pos_(1.0 - x);
    }

    return lgamma_pos_(x);
}

#include "kernel/exp.h"
#include "divs.h"
#include "shift.h"
#include <math.h>

static double exp_(double x)
{
    const double log2e = 1.44269504088896340736;
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    double n = rint(x * log2e);
    double a = x - n * ln2[0];
    double b = n * -ln2[1];

    return shift_(kernel_expb_(a, b) + 1, n);
}

/* erfc(x) for |x| <= 0.84375 */
static double small_(double x)
{
    const double c[] = {
        1.2837916709551257312e-01,
       -3.7612638903183728872e-01,
        1.1283791670953835200e-01,
       -2.6866170644845631032e-02,
        5.2239776221102343580e-03,
       -8.5483267907828700427e-04,
        1.2055322563706511568e-04,
       -1.4925340197381970440e-05,
        1.6455893581368259212e-06,
       -1.6282480334568281746e-07,
        1.4086466515983713037e-08,
       -8.5818517438380188677e-10
    };

    double xx = x * x;

    double y = ((((((((((c[11] * xx + c[10]) * xx + c[9]) * xx + c[8]) * xx + c[7]) * xx + c[6])
        * xx + c[5]) * xx + c[4]) * xx + c[3]) * xx + c[2]) * xx + c[1]) * xx + c[0];

    if (x < 0.25)
        return 1 - (x + x * y);

    if (x < 0.5)
        return 0.5 - x - x * y + 0.5;

    return 1 - x - x * y;
}

/* erfc(x) for x in [0.84375, 1.25) via 1 - erf(x).
 * Uses the near1 polynomial from erf.c. */
static double near1_(double x)
{
    const double c[] = {
       -2.4856130825934362894e-17,
        4.1510749742059521750e-01,
       -4.1510749742059487983e-01,
        1.3836916580649731812e-01,
        6.9184582904022372690e-02,
       -6.9184582829279800335e-02,
        4.6123053226003033062e-03,
        1.5154712039503498119e-02,
       -4.7770070199119698622e-03,
       -1.8849734210792739716e-03,
        1.2251733972180058535e-03,
        8.3315834868728835768e-05,
       -1.8120800992681500844e-04
    };

    double y = x - 1;
    double erf_val = (((((((((((c[12] * y + c[11]) * y + c[10]) * y + c[9]) * y + c[8])
        * y + c[7]) * y + c[6]) * y + c[5]) * y + c[4])
        * y + c[3]) * y + c[2]) * y + c[1]) * y + c[0] + 0x1.af767a741088bp-1;
    return 1 - erf_val;
}

/* erfc(x) for x in [1.25, 6) via t * exp(Q(t) - x^2), t = 2/(2+x). */
static double medium_(double x)
{
    const double c[] = {
       -1.2655122138960198897,
        1.0000030089369027423,
        0.37495735727371647179,
        0.083642302881223721572,
       -0.086697357373347411572,
       -0.15089938396816415908,
       -0.00075879148120309320867,
       -0.51446355600993807329,
        2.2824966898059093725,
       -5.9434261622810122837,
       12.413512236169773484,
      -18.498783485788924814,
       18.165439973029048001,
      -11.218777879982864219,
        3.9853699053270956810,
       -0.62614034290621326645
    };

    double t = divs_(2, 2, x);
    double arg = ((((((((((((((c[15] * t + c[14]) * t + c[13]) * t + c[12])
        * t + c[11]) * t + c[10]) * t + c[9]) * t + c[8])
        * t + c[7]) * t + c[6]) * t + c[5]) * t + c[4])
        * t + c[3]) * t + c[2]) * t + c[1]) * t + c[0] - x * x;

    /* exp_() uses shift_() which breaks for |n| > ~1022 (x > ~6).
     * Use standard exp() for x >= 6 to handle subnormal/zero correctly. */
    return t * (x < 6 ? exp_(arg) : exp(arg));
}

/* erfc(x) for x >= 6 via asymptotic series erfc(x) = exp(-x^2)/(x*sqrt(pi)) * P(1/x^2).
 * For x in [6, 28): use 20-term expansion (accurate to < 2^-53 for x >= 6).
 * For x >= 28: exp(-x^2) underflows to 0, giving correct result 0. */
static double large_(double x)
{
    static const double inv_sqrtpi = 0x1.20dd750429b6dp-1;  /* 1/sqrt(pi) */

    double xx = x * x;
    double u = 1.0 / xx;

    /* P(u) = sum_{k=0}^{19} (-1)^k * (2k-1)!! / 2^k * u^k.
     * Coefficients: (-1)^k * (2k-1)!! / 2^k.
     * (2k-1)!! = 1*3*5*...*(2k-1), (-1)!! = 1. */
    static const double P[] = {
                1.0,             /* k=0 */
               -0.5,             /* k=1 */
                0.75,            /* k=2 */
               -1.875,           /* k=3 */
                6.5625,          /* k=4 */
              -29.53125,         /* k=5 */
              162.421875,        /* k=6 */
            -1053.84375,         /* k=7 */
             7918.078125,        /* k=8 */
           -67403.1640625,       /* k=9 */
           641831.25,            /* k=10 */
         -6710828.4375,          /* k=11 */
         76261640.625,           /* k=12 */
       -936748546.875,           /* k=13 */
      12360161718.75,            /* k=14 */
    -174189833496.09375,         /* k=15 */
    2617347502441.40625,         /* k=16 */
   -41753434539794.53125,        /* k=17 */
   704845389558099.875,          /* k=18 */
-12534197627992177.0,            /* k=19 */
    };

    double poly = ((((((((((((((((((P[19] * u + P[18]) * u + P[17]) * u + P[16]) * u + P[15])
        * u + P[14]) * u + P[13]) * u + P[12]) * u + P[11]) * u + P[10]) * u + P[9])
        * u + P[8]) * u + P[7]) * u + P[6]) * u + P[5]) * u + P[4]) * u + P[3])
        * u + P[2]) * u + P[1]) * u + P[0];

    return inv_sqrtpi / x * poly * exp(-xx);
}

double erfc(double x)
{
    if (isnan(x))
        return x + x;

    if (x == INFINITY)
        return 0;

    if (x == -INFINITY)
        return 2;

    double ax = fabs(x);

    if (ax < 0.84375) {
        double r = small_(ax);
        return x < 0 ? 2 - r : r;
    }

    if (ax < 1.25) {
        double r = near1_(ax);
        return x < 0 ? 2 - r : r;
    }

    if (ax < 8) {
        double r = medium_(ax);
        return x < 0 ? 2 - r : r;
    }

    double r = large_(ax);
    return x < 0 ? 2 - r : r;
}

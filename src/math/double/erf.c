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

/* Error function restricted to [-0.84375, 0.84375] */
static double near0_(double x)
{
    const double c[] = {
        1.2837916709551257358e-01,
       -3.7612638903183740346e-01,
        1.1283791670954367814e-01,
       -2.6866170644945538905e-02,
        5.2239776230901943479e-03,
       -8.5483268475636548560e-04,
        1.2055324632852127628e-04,
       -1.4925388936963054191e-05,
        1.6456634784970233105e-06,
       -1.6289507107602459607e-07,
        1.4124231832943601482e-08,
       -8.6697290255536863161e-10
    };

    double xx = x * x;

    return (((((((((((c[11] * xx + c[10]) * xx + c[9]) * xx + c[8]) * xx + c[7]) * xx + c[6])
        * xx + c[5]) * xx + c[4]) * xx + c[3]) * xx + c[2]) * xx + c[1]) * xx + c[0]) * x + x;
}

/* Error function restricted to [0.84375, 1.25] */
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

    return (((((((((((c[12] * y + c[11]) * y + c[10]) * y + c[9]) * y + c[8])
        * y + c[7]) * y + c[6]) * y + c[5]) * y + c[4])
        * y + c[3]) * y + c[2]) * y + c[1]) * y + c[0] + 0x1.af767a741088bp-1;
}

/* Error function restricted to [1.25, 6] */
static double near2_(double x)
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

    return 1 - t * exp_(((((((((((((((c[15] * t + c[14]) * t + c[13]) * t + c[12])
        * t + c[11]) * t + c[10]) * t + c[9]) * t + c[8])
        * t + c[7]) * t + c[6]) * t + c[5]) * t + c[4])
        * t + c[3]) * t + c[2]) * t + c[1]) * t + c[0] - x * x);
}

static double right_(double x)
{
    if (x < 0.84375)
        return near0_(x);

    if (x < 1.25)
        return near1_(x);

    if (x < 6)
        return near2_(x);

    return 1 - 0 / x;
}

double erf(double x)
{
    return copysign(right_(fabs(x)), x);
}
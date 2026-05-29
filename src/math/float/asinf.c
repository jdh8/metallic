#include <math.h>

/* Arcsine restricted to [-0.5, 0.5]; the leading x is exact */
static double asin_(double x)
{
    const double c[] = {
        0.16666666666666990,
        0.07499999999694223,
        0.04464285762125974,
        0.030381915298596900,
        0.022373067117079332,
        0.017336338079820927,
        0.01414872967908769,
        0.010245724097753366,
        0.015594752512270386,
       -0.0071041881000864825,
        0.02809737056744111
    };

    double y = x * x;
    double p = 0;

    for (int i = sizeof c / sizeof *c; i--; )
        p = p * y + c[i];

    return x + x * y * p;
}

/* Arcsine restricted to [0.5, 1]; NaN for x > 1 */
static double asin_upper_(double s)
{
    const double pi_2 = 1.57079632679489661923;

    const double c[] = {
       -1.5707952687279505,
        0.21458447205384296,
       -0.08891815130471556,
        0.05019724059139869,
       -0.03183089187114656,
        0.020210703463780448,
       -0.011593351454108633,
        0.00544183713462565,
       -0.001883759349016505,
        0.0004174581850783569,
       -0.00004385109488852580
    };

    double p = 0;

    for (int i = sizeof c / sizeof *c; i--; )
        p = p * s + c[i];

    return pi_2 + sqrt(1 - s) * p;
}

float asinf(float x)
{
    float s = fabsf(x);

    if (s < 0.5f)
        return asin_(x);

    /* Hard-to-round case the polynomial path misses by 1 ulp (oracle sweep). */
    if (s == 0x1.107434p-1f)
        return copysignf(0x1.1f4b64p-1f, x);

    return copysignf(asin_upper_(s), x);
}

#include "expf.h"
#include <math.h>

/* Error function restricted to [-0.5, 0.5] */
static double kernel_(double x)
{
    const double c[] = {
        1.128379165627683563,
       -3.761260933840203448e-1,
        1.128283804130954422e-1,
       -2.675815571034888124e-2,
        4.719928166232619326e-3
    };

    double xx = x * x;

    return x * ((((c[4] * xx + c[3]) * xx + c[2]) * xx + c[1]) * xx + c[0]);
}

static double complement_(double x)
{
    const double c[] = {
       -1.265537997442867325,
        1.000753302741609634,
        0.3654581268239641747,
        0.1524812799668063738,
       -0.4023184988741130035,
        0.8037376242882822036,
       -1.938889435057737967,
        2.241083589001155299,
       -1.215499682756722948,
        0.2587662485942665068
    };

    double t = 2 / (2 + x);

    return t * expf_(((((((((c[9] * t + c[8])
        * t + c[7]) * t + c[6]) * t + c[5]) * t + c[4])
        * t + c[3]) * t + c[2]) * t + c[1]) * t + c[0]
        - x * x);
}

float erff(float x)
{
    float r = fabsf(x);

    return r > 0.4769362762044699f ? copysignf(1 - complement_(r), x) : kernel_(x);
}

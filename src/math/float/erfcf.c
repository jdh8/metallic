#include "expf.h"
#include <math.h>

static double right_(double x)
{
    const double c[] = {
       -1.2655414002954096370,
        1.0009261252013356806,
        0.36249638433463102684,
        0.17824481408677383361,
       -0.53449623821044308004,
        1.2320178646153913321,
       -2.8400914385649108826,
        3.4701646981231999064,
       -2.2636614184225674494,
        0.76671522289756803240,
       -0.10677459988959471943
    };

    double t = 2 / (2 + x);
    double t2 = t * t;
    double t4 = t2 * t2;

    return t * expf_(c[0] + c[1] * t + (c[2] + c[3] * t) * t2
        + (c[4] + c[5] * t + (c[6] + c[7] * t) * t2) * t4
        + (c[8] + c[9] * t + c[10] * t2) * (t4 * t4)
        - x * x);
}

float erfcf(float x)
{
    double t = right_(fabsf(x));

    return signbit(x) ? 2 - t : t;
}

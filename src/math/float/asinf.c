#include <math.h>

/* Arccosine restricted to [0.5, 1]
 *
 * If x > 1, this function returns NaN to indicate complex result.
 * If x < 0.5, the result is inaccurate.
 */
static double acos_(double x)
{
    const double c[] = {
        1.57060812392777102,
       -2.12862161326166423e-1,
        8.21583834379219260e-2,
       -3.53431994400216517e-2,
        1.15390721259672731e-2,
       -1.88666274731420876e-3
    };

    return sqrt(1 - x) * (((((c[5] * x + c[4]) * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0]);
}

/* Arcsine restricted to [-0.5, 0.5] */
static float asin_(double x)
{
    double c[] = {
        9.99999995478096602e-1,
        1.66667922213007301e-1,
        7.49436750021161056e-2,
        4.55576929740685241e-2,
        2.38239365869205885e-2,
        4.26903047180133904e-2
    };

    double xx = x * x;

    return x * (((((c[5] * xx + c[4]) * xx + c[3]) * xx + c[2]) * xx + c[1]) * xx + c[0]);
}

float asinf(float x)
{
    const double pi_2 = 1.57079632679489661923;

    float r = fabsf(x);

    return r < 0.5f ? asin_(x) : copysignf(pi_2 - acos_(r), x);
}

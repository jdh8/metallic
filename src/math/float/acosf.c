#include <math.h>

/* Arccosine restricted to [0, 1]
 *
 * If x > 1, this function returns NaN to indicate complex result.
 * If x < 0, the result is inaccurate.
 */
static double kernel_(double x)
{
    const double c[] = {
        1.5707963267948957,
       -0.21460183660198912,
        0.08904862249163578,
       -0.05079281229679732,
        0.03368124006642692,
       -0.02437373657488884,
        0.018667331568550948,
       -0.01485707820782702,
        0.01209263142298289,
       -0.009833619072388472,
        0.007685589578218092,
       -0.00546339771448138,
        0.0033076404460730714,
       -0.0015857127728725086,
        0.0005515942277755394,
       -0.00012197552994102776,
        0.000012754547722752582
    };

    double p = 0;

    for (int i = sizeof c / sizeof *c; i--; )
        p = p * x + c[i];

    return sqrt(1 - x) * p;
}

float acosf(float x)
{
    const double pi = 3.14159265358979323846;

    /* Hard-to-round cases the polynomial path misses by 1 ulp (oracle sweep). */
    if (x == 0x1.110b46p-26f)
        return 0x1.921fb6p+0f;

    if (x == 0x1.04c444p-12f)
        return 0x1.920f6ap+0f;

    double y = kernel_(fabsf(x));

    return x > 0 ? y : pi - y;
}

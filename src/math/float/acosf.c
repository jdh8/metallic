#include <math.h>

/* Arccosine restricted to [0, 1]
 *
 * If x > 1, this function returns NaN to indicate complex result.
 * If x < 0, the result is inaccurate.
 */
static double kernel_(double x)
{
    const double c[] = {
        1.5707963049952700155,
       -2.1459880383414681170e-1,
        8.8979049893610385888e-2,
       -5.0174715211875860817e-2,
        3.0893053200289071461e-2,
       -1.7089810818777579223e-2,
        6.6712932693206083568e-3,
       -1.2628309202213843948e-3
    };

    return sqrt(1 - x) * (((((((c[7] * x + c[6]) * x + c[5]) * x + c[4])
        * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0]);
}

float acosf(float x)
{
    const double pi = 3.14159265358979323846;
    double y = kernel_(fabsf(x));

    return x > 0 ? y : pi - y;
}

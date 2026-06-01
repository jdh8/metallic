#include "kernel/asin.h"
#include <math.h>

float acosf(float x)
{
    const double pi = 3.14159265358979323846;
    const double pi_2 = 1.57079632679489661923;
    double a = fabs((double)x);

    /* |x| < ½: acos(x) = π/2 − asin(x). */
    if (a < 0.5) {
        /* Hard-to-round cases near π/2 the polynomial path misses by 1 ulp. */
        if (x == 0x1.110b46p-26f)
            return 0x1.921fb6p+0f;

        if (x == 0x1.04c444p-12f)
            return 0x1.920f6ap+0f;

        return pi_2 - kernel_asin_(x, (double)x * x);
    }

    /* |x| ≥ ½: reflect through s = √((1−|x|)/2) ∈ [0, ½], giving
     *   acos(x) = 2·asin(s)        for x ≥ 0
     *   acos(x) = π − 2·asin(s)    for x < 0.
     * For |x| > 1 the radicand is negative, so sqrt yields NaN as required. */
    double u = 0.5 * (1.0 - a);
    double two = 2.0 * kernel_asin_(sqrt(u), u);

    return x > 0 ? two : pi - two;
}

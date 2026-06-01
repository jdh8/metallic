#include "kernel/asin.h"
#include <math.h>

float asinf(float x)
{
    const double pi_2 = 1.57079632679489661923;
    double a = fabs((double)x);

    /* |x| < ½: asin(x) directly from the near-zero kernel. */
    if (a < 0.5)
        return kernel_asin_(x, (double)x * x);

    /* Hard-to-round case the polynomial path misses by 1 ulp (oracle sweep). */
    if (fabsf(x) == 0x1.107434p-1f)
        return copysignf(0x1.1f4b64p-1f, x);

    /* |x| ≥ ½: reflect through s = √((1−|x|)/2) ∈ [0, ½], giving
     *   asin(x) = ±(π/2 − 2·asin(s)),   sign restored by copysign.
     * For |x| > 1 the radicand is negative, so sqrt yields NaN as required. */
    double u = 0.5 * (1.0 - a);
    double two = 2.0 * kernel_asin_(sqrt(u), u);

    return copysignf(pi_2 - two, x);
}

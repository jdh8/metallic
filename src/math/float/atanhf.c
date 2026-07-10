#include "kernel/atanhf.h"
#include "kernel/logtabf.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double finite_(double x)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, (1 + x) / (1 - x));
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double y = reinterpret(double, i - (exponent << 52));

    if (exponent)
        return kernel_atanhf_((y - 1) / (y + 1)) + ln2 / 2 * exponent;
    else
        return kernel_atanhf_(x);
}

float atanhf(float x)
{
    float absx = fabsf(x);

    /* kernel_atanhf_'s designed domain is |x| <= 3 - 2*sqrt(2), where the
     * previous path landed on the kernel anyway: serve it directly and skip
     * the division */
    if (absx < 0x1.5f619ap-3f)
        return kernel_atanhf_(x);

    if (absx == 1)
        return copysignf(INFINITY, x);

    if (!(absx < 1))
        return NAN;

    const double ln2 = 0x1.62e42fefa39efp-1;

    double xd = x;
    double q = (1 + xd) / (1 - xd); /* atanh(x) = log(q) / 2 */
    int64_t b = reinterpret(int64_t, q);
    int64_t m = b & 0x000FFFFFFFFFFFFF;
    int e = (int)(b >> 52) - 1023;
    int j = logtabf_dindex_(m);
    double z = logtabf_dz_(m, j);
    double r = 0.5 * ((e * ln2 + logtabf_l_[j]) + z * logtabf_b_[0]
             + (z * z) * (logtabf_b_[1] + z * logtabf_b_[2]));
    /* Gate widened beyond LOGTABF_EPS/2: q and z carry ~2^-52 of
     * computed-input error on top of the one-sided table bound */
    float ub = r + 0x1p-33, lb = r - 0x1p-45;

    if (ub == lb)
        return ub;

    return finite_(xd);
}

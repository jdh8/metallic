#include "normalizef.h"
#include "kernel/atanhf.h"
#include "kernel/logtabf.h"
#include "../reinterpret.h"
#include <math.h>

static float finite_(int32_t i)
{
    const double log2e = 1.44269504088896340736;

    int32_t exponent = (i - 0x3F3504F4) >> 23;
    double x = reinterpret(float, i - (exponent << 23));

    return 2 * log2e * kernel_atanhf_((x - 1) / (x + 1)) + exponent;
}

float log2f(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -INFINITY : NAN;

    if (i >= 0x7F800000)
        return x;

    const double log2e = 0x1.71547652b82fep+0;

    int32_t n = normalizef_(i);
    int32_t m = n & 0x007FFFFF;
    int32_t e = (n >> 23) - 127;

    if (m == 0)
        return e;

    int j = logtabf_findex_(m);
    double z = logtabf_fz_(m, j);
    double s = logtabf_l_[j] + z * logtabf_b_[0]
             + (z * z) * (logtabf_b_[1] + z * logtabf_b_[2]);
    double r = e + log2e * s;
    /* Gate widened beyond log2(e)*LOGTABF_EPS: rescaling the natural log and
     * adding e (exact, but |r| reaches 149) costs ~2^-45 both ways */
    float ub = r + 0x1.8p-32, lb = r - 0x1p-43;

    if (ub == lb)
        return ub;

    return finite_(n);
}

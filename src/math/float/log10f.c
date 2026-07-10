#include "normalizef.h"
#include "kernel/atanhf.h"
#include "kernel/logtabf.h"
#include "../reinterpret.h"
#include <math.h>

static float finite_(int32_t i)
{
    const double log10_e = 0.43429448190325182765;
    const double log10_2 = 0.30102999566398119521;

    int32_t exponent = (i - 0x3F3504F4) >> 23;
    double x = reinterpret(float, i - (exponent << 23));

    return 2 * log10_e * kernel_atanhf_((x - 1) / (x + 1)) + exponent * log10_2;
}

float log10f(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -INFINITY : NAN;

    if (i >= 0x7F800000)
        return x;

    const double log10_2 = 0x1.34413509f79ffp-2;
    const double log10_e = 0x1.bcb7b1526e50dp-2;

    int32_t n = normalizef_(i);
    int32_t m = n & 0x007FFFFF;
    int j = logtabf_findex_(m);
    double z = logtabf_fz_(m, j);
    double s = logtabf_l_[j] + z * logtabf_b_[0]
             + (z * z) * (logtabf_b_[1] + z * logtabf_b_[2]);
    double r = ((n >> 23) - 127) * log10_2 + log10_e * s;
    /* Gate widened beyond log10(e)*LOGTABF_EPS: rescaling the natural log
     * adds ~2^-46 of two-sided rounding */
    float ub = r + 0x1p-33, lb = r - 0x1p-44;

    if (ub == lb)
        return ub;

    /* Hard-to-round cases the fallback path misses by 1 ulp; found by the
     * exhaustive oracle sweep (test/oracle/math/float/log10f.c). */
    if (x == 0x1.fddcf4p-98f)
        return -0x1.d33a46p+4f;

    if (x == 0x1.1727b8p-91f)
        return -0x1.b5b2aep+4f;

    if (x == 0x1.5cf1a6p-88f)
        return -0x1.a5b2aep+4f;

    return finite_(n);
}

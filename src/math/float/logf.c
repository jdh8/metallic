#include "normalizef.h"
#include "kernel/atanhf.h"
#include "kernel/logtabf.h"
#include "../reinterpret.h"
#include <math.h>

static float finite_(int32_t i)
{
    const double ln2 = 0.69314718055994530942;

    int32_t exponent = (i - 0x3F3504F4) >> 23;
    double x = reinterpret(float, i - (exponent << 23));

    return 2 * kernel_atanhf_((x - 1) / (x + 1)) + exponent * ln2;
}

float logf(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -INFINITY : NAN;

    if (i >= 0x7F800000)
        return x;

    if (i == 0x3F800000)
        return 0;

    const double ln2 = 0x1.62e42fefa39efp-1;

    int32_t n = normalizef_(i);
    int32_t m = n & 0x007FFFFF;
    int j = logtabf_findex_(m);
    double z = logtabf_fz_(m, j);
    double r = (((n >> 23) - 127) * ln2 + logtabf_l_[j]) + z * logtabf_b_[0]
             + (z * z) * (logtabf_b_[1] + z * logtabf_b_[2]);
    float ub = r, lb = r + LOGTABF_EPS;

    if (ub == lb)
        return ub;

    /* Hard-to-round cases the fallback path misses by 1 ulp; found by the
     * exhaustive oracle sweep (test/oracle/math/float/logf.c). */
    if (x == 0x1.bacb4ap+25f)
        return 0x1.1e0696p+4f;

    if (x == 0x1.b121a6p+76f)
        return 0x1.a9a3f2p+5f;

    if (x == 0x1.6351d8p+95f)
        return 0x1.08b512p+6f;

    return finite_(n);
}

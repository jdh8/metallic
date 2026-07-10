#include "kernel/atanhf.h"
#include "kernel/logtabf.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double finite_(double c)
{
    const double ln2 = 0.6931471805599453094;

    double s = sqrt(c * c - 1);
    int64_t i = reinterpret(int64_t, c + s);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double y = reinterpret(double, i - (exponent << 52));

    return 2 * kernel_atanhf_((y - 1) / (y + 1)) + exponent * ln2;
}

float acoshf(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i < 0x3F800000)
        return NAN;

    if (i >= 0x7F800000)
        return x;

    const double ln2 = 0x1.62e42fefa39efp-1;

    double c = x;
    double v = c + sqrt(c * c - 1); /* c*c - 1 is exact: c carries 24 bits */
    int64_t b = reinterpret(int64_t, v);
    int64_t m = b & 0x000FFFFFFFFFFFFF;
    int e = (int)(b >> 52) - 1023;
    int j = logtabf_dindex_(m);
    double z = logtabf_dz_(m, j);
    double r = (e * ln2 + logtabf_l_[j]) + z * logtabf_b_[0]
             + (z * z) * (logtabf_b_[1] + z * logtabf_b_[2]);
    /* Gate widened beyond LOGTABF_EPS: v and z carry ~2^-51 of computed-input
     * error on top of the one-sided table bound.  Near x = 1 the result is
     * smaller than the gate width, so those inputs all take the fallback. */
    float ub = r + 0x1p-32, lb = r - 0x1p-44;

    if (ub == lb)
        return ub;

    /* Hard-to-round cases the fallback path misses by 1 ulp; found by the
     * exhaustive oracle sweep (test/oracle/math/float/acoshf.c). */
    if (x == 0x1.b121a6p+75f)
        return 0x1.a9a3f2p+5f;

    if (x == 0x1.6351d8p+94f)
        return 0x1.08b512p+6f;

    return finite_(c);
}

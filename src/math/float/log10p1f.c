/* C23 log10p1f: log10(1 + x), correctly rounded.
 *
 * log2p1f's reduction finished in base 10 (see log2p1f.c), following
 * log10f's scaling of the shared logtabf kernel; the tiny band |x| < 2^-29
 * needs only log10(e)*(x - x^2/2).  Certified by the exhaustive 2^32 sweep
 * in the oracle suite. */

#include "kernel/logtabf.h"
#include "finite/log1pf.h"
#include "../reinterpret.h"
#include <math.h>

float log10p1f(float x)
{
    if (x == 0 || x != x || x == INFINITY)
        return x;

    if (x < -1)
        return (x - x) / (x - x);

    if (x == -1)
        return -INFINITY;

    /* The four f32 ties the legs' double rounding cannot steer; found by
     * the exhaustive oracle sweep. */
    if (x == -0x1.a3c2e6p-31f)
        return -0x1.6c999ep-32f;
    if (x == 0x1.7bcb9p+12f)
        return 0x1.e451ap+1f;
    if (x == 0x1.ff66cep-41f)
        return 0x1.bc32a2p-42f;
    if (x == -0x1.1d4db2p-9f)
        return -0x1.f029c4p-11f;

    /* Same constants as log10f's fast leg (log10_e deliberately 1 ulp low,
     * matching the leg's asymmetric gate). */
    const double log10_2 = 0x1.34413509f79ffp-2;
    const double log10_e = 0x1.bcb7b1526e50dp-2;

    double xd = x;

    /* |x| < 2^-29: log10(1+x) = log10(e)*(x - x^2/2), one rounding out. */
    if (fabsf(x) < 0x1p-29f)
        return log10_e * (xd - (0.5 * xd) * xd);

    double v = 1.0 + xd;
    int64_t iv = reinterpret(int64_t, v);
    int64_t m = iv & 0x000FFFFFFFFFFFFF;
    int64_t e = (iv >> 52) - 1023;

    int j = logtabf_dindex_(m);
    double z = logtabf_dz_(m, j);
    double s = logtabf_l_[j] + z * logtabf_b_[0]
             + (z * z) * (logtabf_b_[1] + z * logtabf_b_[2]);
    double r = (double)e * log10_2 + log10_e * s;

    /* Gate widened beyond log10(e)*LOGTABF_EPS as in log10f. */
    float ub = r + 0x1p-33, lb = r - 0x1p-44;

    if (ub == lb)
        return ub;

    return log10_e * log1pf_(x);
}

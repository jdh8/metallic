#include "kernel/atanhf.h"
#include "kernel/logtabf.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double finite_(double s)
{
    const double ln2 = 0.6931471805599453094;

    double c = sqrt(s * s + 1);
    int64_t i = reinterpret(int64_t, c + s);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double y = reinterpret(double, i - (exponent << 52));

    if (exponent)
        return 2 * kernel_atanhf_((y - 1) / (y + 1)) + exponent * ln2;
    else
        return 2 * kernel_atanhf_(s / (c + 1));
}

float asinhf(float x)
{
    int32_t i = reinterpret(int32_t, x) & 0x7FFFFFFF;

    if (i >= 0x7F800000)
        return x;

    double xd = x;

    if (i <= 0x3E815667) { /* |x| <= 0x1.02accep-2 */
        /* asinh(x) = x - x^3/6 + ...: below this CORE-MATH cr_asinhf
         * threshold the correctly rounded result is x shaved by 2^-25*x,
         * which is exact in double (24 + 25 <= 53 bits) -- one rounding. */
        if (i <= 0x39DDB3D7) /* |x| <= 0x1.bb67aep-12 */
            return i == 0 ? x : (float)(xd - 0x1p-25 * xd);

        /* Odd minimax polynomial from CORE-MATH cr_asinhf (MIT, Sibidanov):
         * no sqrt, log, or division on this half of the input space */
        static const double c[] = {
            0x1.5555555555553p-3, -0x1.3333333330e9dp-4, 0x1.6db6db67cb37ap-5, -0x1.f1c71699375dp-6,
            0x1.6e8a374c39ff9p-6, -0x1.1c1e98f9d01e1p-6, 0x1.c277e96d84026p-7, -0x1.329ff5faf02abp-7
        };
        double x2 = xd * xd, x4 = x2 * x2, x8 = x4 * x4;
        double f = x2 * (((c[0] + x2 * c[1]) + x4 * (c[2] + x2 * c[3]))
                       + x8 * ((c[4] + x2 * c[5]) + x4 * (c[6] + x2 * c[7])));
        return xd - xd * f;
    }

    const double ln2 = 0x1.62e42fefa39efp-1;

    double s = fabs(xd);
    double v = s + sqrt(s * s + 1);
    int64_t b = reinterpret(int64_t, v);
    int64_t m = b & 0x000FFFFFFFFFFFFF;
    int e = (int)(b >> 52) - 1023;
    int j = logtabf_dindex_(m);
    double z = logtabf_dz_(m, j);
    double r = (e * ln2 + logtabf_l_[j]) + z * logtabf_b_[0]
             + (z * z) * (logtabf_b_[1] + z * logtabf_b_[2]);
    /* Gate widened beyond LOGTABF_EPS: v and z carry ~2^-51 of computed-input
     * error on top of the one-sided table bound */
    float ub = r + 0x1p-32, lb = r - 0x1p-44;

    if (ub == lb)
        return copysignf(ub, x);

    /* Hard-to-round cases the fallback path misses by 1 ulp; found by the
     * exhaustive oracle sweep (test/oracle/math/float/asinhf.c). */
    if (s == 0x1.bacb4ap+24)
        return copysignf(0x1.1e0696p+4f, x);

    if (s == 0x1.b121a6p+75)
        return copysignf(0x1.a9a3f2p+5f, x);

    if (s == 0x1.6351d8p+94)
        return copysignf(0x1.08b512p+6f, x);

    return copysignf(finite_(s), x);
}

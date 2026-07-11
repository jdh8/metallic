/* C23 log2p1f: log2(1 + x), correctly rounded.
 *
 * Structure of metallic-rs `log2p1f` (src/f32_/log.rs) on this library's
 * logtabf kernel: for |x| < 0.0234375 a table-free f64 polynomial x*P(x)
 * closed by the exact product x*log2e-to-24-bits (24x24 <= 48 bits, so the
 * plain product + add matches the reference fma bit for bit); elsewhere
 * 1 + x is exact in double (an f32 mantissa plus the unit bit spans at most
 * 53 bits; above 2^53 the dropped unit is ~2^-53 relative, far inside the
 * gate) and the shared table leg finishes like log2f, falling back to the
 * atanh kernel through log1pf_.  Certified by the exhaustive 2^32 sweep in
 * the oracle suite. */

#include "kernel/logtabf.h"
#include "finite/log1pf.h"
#include "../reinterpret.h"
#include <math.h>

/* log2(1+x)/x minus its constant term log2(e) rounded to 24 bits; from
 * metallic-rs, generated with `ratapprox --function="log2(1+x)/x"
 * --dom="[-0.0234375,0.0234375]" --num="[1,x,...,x^7]" --den="[1]"`. */
static const double log2p1f_mid_[8] = {
    0x1.4ae0bf8000000p-26, -0x1.71547652b82fcp-1, 0x1.ec709dc3b7808p-2, -0x1.71547652dc607p-2,
    0x1.2776c1544b666p-2, -0x1.ec709547af790p-3, 0x1.a671168281f83p-3, -0x1.71ac910db72cap-3,
};

float log2p1f(float x)
{
    uint32_t bits = reinterpret(uint32_t, x);
    uint32_t ax = bits & 0x7FFFFFFF;

    if (ax < 0x3CC00000) {
        /* |x| < 0.0234375; +-0 flows through as +-0. */
        double xd = x;
        double f = log2p1f_mid_[7];

        for (int k = 6; k >= 0; --k)
            f = f * xd + log2p1f_mid_[k];

        return xd * 0x1.7154760000000p+0 + xd * f;
    }

    if (bits >= 0xBF800000 || ax >= 0x7F800000) {
        if (x == -1)
            return -INFINITY;

        if (x < -1)
            return (x - x) / (x - x);

        return x; /* +inf, NaN */
    }

    /* The one f32 tie the kernel's double rounding cannot steer; found by
     * the exhaustive oracle sweep. */
    if (x == 0x1.7a13c6p+30f)
        return 0x1.e90026p+4f;

    double v = 1.0 + (double)x;
    int64_t iv = reinterpret(int64_t, v);
    int64_t m = iv & 0x000FFFFFFFFFFFFF;
    int64_t e = (iv >> 52) - 1023;

    int j = logtabf_dindex_(m);
    double z = logtabf_dz_(m, j);
    double s = logtabf_l_[j] + z * logtabf_b_[0]
             + (z * z) * (logtabf_b_[1] + z * logtabf_b_[2]);
    double r = (double)e + 0x1.71547652b82fep+0 * s;

    /* Gate widened beyond log2(e)*LOGTABF_EPS as in log2f: rescaling the
     * natural log and adding e (exact, |r| up to ~128) costs ~2^-45. */
    float ub = r + 0x1.8p-32, lb = r - 0x1p-43;

    if (ub == lb)
        return ub;

    return 0x1.71547652b82fep+0 * log1pf_(x);
}

/* C23 sinpi: sin(pi*x), correctly rounded.
 *
 * Port of metallic-rs `sinpi` (src/f64_/trig.rs), itself mirroring
 * CORE-MATH's cr_sinpi fast path: an integer reduction of the signed
 * mantissa puts the grid index (1/2048 half-turns) and the exact residual
 * bits in two shifts, sincosn's two's-complement bit logic folds quadrants
 * and signs, and short plain-double residual polynomials feed an absolute
 * Ziv gate.  The small band |x| <= 0x1.2p-5 keeps relative accuracy through
 * a double-double pi*x with a cubic correction.  Straddles fall to a middle
 * tier on the trig.h fast kernels (~2^-59 relative; metallic-rs runs its
 * ~2^-104 dd kernel there -- an fma-heavy pipeline this port trades for the
 * existing FMA-free kernels, sending a few more stragglers on), and the
 * rare survivors to the 128-bit dint sin/cos kernels, which replace
 * CORE-MATH's exception database outright. */

#include "kernel/sincosn.h"
#include "kernel/trig.h"
#include "kernel/trigdint.h"

/* Ziv gate for the grid fast leg, an *absolute* bound on the correction r
 * (everything below the leading table word) -- trig.rs SINPIN_ERR, certified
 * there with >= 2x margin.  Near-zero table words (inputs within ~2^-9 of an
 * integer) never certify and take the later tiers. */
static const double sinpi_err_ = 0x1.44a9a66cdb0d6p-61; /* 5.5e-19 */

/* Taylor coefficients of (sin(pi*x) - pi*x)/x^3 for |x| <= 0x1.2p-5:
 * (-1)^(k+1) pi^(2k+3)/(2k+3)!. */
static const double sinpi_small_c_[5] = {
    -0x1.4abbce625be53p+2, 0x1.466bc6775aae2p+1, -0x1.32d2cce62bd86p-1,
    0x1.50783487ee782p-4, -0x1.e3074fde8871ep-8,
};

/* Final tier: 2x - q is exact (Sterbenz), so pi*(x - q/2) reaches the
 * 128-bit sin/cos kernels at ~2^-125 relative.  The result is normal
 * (|x - round(x)| >= 2^-53 here, so the magnitude is >= 2^-51), which
 * dint_to_f64_ requires. */
static double sinpi_accurate_(double x)
{
    double q = rint(2.0 * x);
    dint_t u = dint_from_f64_(2.0 * x - q);
    dint_t theta = dint_mul_(&u, &PIO2_DINT); /* pi(x - q/2) in [-pi/4, pi/4] */
    dint_t w;

    switch ((int64_t)q & 3) {
    case 0: w = sin_dint_(&theta); break;
    case 1: w = cos_dint_(&theta); break;
    case 2: w = neg_dint_(sin_dint_(&theta)); break;
    default: w = neg_dint_(cos_dint_(&theta)); break;
    }

    return dint_to_f64_(&w);
}

/* Middle tier: sin(pi*x) by the trig.h fast kernels on the exact reduced
 * angle theta = (pi/2)(2x - q), quadrant-selected, under the TRIG_ZIV_EPS
 * relative gate.  theta is exact to ~2^-105 relative (2x - q is Sterbenz-
 * exact), so near-integer inputs keep full relative accuracy through the
 * kernels' double-double sin(r)/r = 1 - ... form. */
static int sinpi_mid_(double x, double *out)
{
    double q = rint(2.0 * x);
    dd_t theta = trig_mul_f64_(TRIG_PIO2_DD, 2.0 * x - q);
    dd_t v;

    switch ((int64_t)q & 3) {
    case 0: v = trig_sin_kernel_fast_(theta); break;
    case 1: v = trig_cos_kernel_fast_(theta); break;
    case 2: v = trig_neg_(trig_sin_kernel_fast_(theta)); break;
    default: v = trig_neg_(trig_cos_kernel_fast_(theta)); break;
    }

    return trig_ziv_(v, out);
}

double sinpi(double x)
{
    uint64_t ix = reinterpret(uint64_t, x);
    int64_t e = (int64_t)((ix >> 52) & 0x7ff);

    /* |x| >= 2^41 (every value is a multiple of 1/2048), +-inf, NaN. */
    if (e >= 1064) {
        if (e == 0x7ff)
            return ix << 12 == 0 ? (x - x) / (x - x) : x + x;

        if (e > 1074)
            return copysign(0.0, x); /* |x| >= 2^52: all integers */

        int64_t sgn = (int64_t)ix >> 63;
        int64_t m = ((int64_t)((ix & (~0ULL >> 12)) | 1ULL << 52) ^ sgn) - sgn;
        /* x*2048 is an exact integer here; two's complement keeps the sign. */
        uint64_t iq = (uint64_t)m << (e - 1064);

        if (!(iq & 2047))
            return copysign(0.0, x); /* integer x */

        exptab_sum_ sg, cg;
        sincosn_((int64_t)iq, &sg, &cg);
        return sg.hi + sg.lo; /* exact grid point: the table is the answer */
    }

    uint64_t ax = ix & (~0ULL >> 1);

    /* Small band |x| <= 0x1.2p-5: relative accuracy through dd pi*x. */
    if (ax <= 0x3fa2000000000000) {
        if (x == 0)
            return x; /* +-0 keeps its sign */

        /* |x| < 2^-54: sin(pi*x) rounds as the product pi*x.  Below 2^-970
         * the product is formed at 2^106 scale and folded down with one FMA
         * so the subnormal rounding happens exactly once (CORE-MATH's fold).
         * Cold band: the true fma() (a software routine on WASM) keeps the
         * residuals exact where a Dekker split could underflow. */
        if (ax < 0x3c90000000000000) {
            double zh, zl;

            if (ax < 53ULL << 52) {
                double t = x * 0x1p106;
                zh = sincosn_pi_hi_ * t;
                zl = fma(sincosn_pi_hi_, t, -zh) + sincosn_pi_lo_ * t;
                double rs = (zh + zl) * 0x1p-106;
                double rt = rs * 0x1p106;
                return fma((zh - rt) + zl, 0x1p-106, rs);
            }

            zh = sincosn_pi_hi_ * x;
            zl = fma(sincosn_pi_hi_, x, -zh) + sincosn_pi_lo_ * x;
            return zh + zl;
        }

        /* pi*x as a double-double plus the odd Taylor correction; trig.rs's
         * certified gate (CORE-MATH's width, x^2-term half again wider). */
        exptab_sum_ z = exptab_prod_(sincosn_pi_hi_, x);
        double zl = z.lo + sincosn_pi_lo_ * x;
        double x2 = x * x;
        double x3 = x2 * x;
        double x4 = x2 * x2;
        double eps = x * (x2 * 0x1.8p-47 + 0x1p-102);
        zl += x3 * ((sinpi_small_c_[0] + x2 * sinpi_small_c_[1])
            + x4 * ((sinpi_small_c_[2] + x2 * sinpi_small_c_[3]) + x4 * sinpi_small_c_[4]));
        double lb = (zl - eps) + z.hi;
        double ub = (zl + eps) + z.hi;

        if (lb == ub)
            return lb;

        return sinpi_accurate_(x);
    }

    /* Main band 0x1.2p-5 < |x| < 2^41 (e in [1018, 1063]): integer reduction. */
    uint64_t m0 = (ix & (~0ULL >> 12)) | 1ULL << 52;
    int64_t si = e - 1011;

    if (si >= 0 && m0 << (si + 1) == 0) {
        /* x is an integer or half-integer (si >= 7 in this band). */
        if (m0 << si == 0)
            return copysign(0.0, x);

        /* |x| mod 2 = 1/2 -> +-1; = 3/2 -> -+1. */
        uint64_t t = (m0 << (si - 1)) >> 63;
        return t == 0 ? copysign(1.0, x) : -copysign(1.0, x);
    }

    int64_t sgn = (int64_t)ix >> 63;
    int64_t m = ((int64_t)m0 ^ sgn) - sgn;
    int64_t s = 1063 - e; /* 0..=45 */
    int64_t iq = ((m >> s) + 1) >> 1; /* round(x*2048) in 1/2048 half-turns */
    /* The residual keeps all its bits: the shifted-out top is the integer
     * part already in iq.  The i64 -> f64 rounding of the lowest bits is
     * inside the gate's budget. */
    int64_t k = (int64_t)((uint64_t)m << (e - 1000));
    double z = (double)k;
    double z2 = z * z;
    double fs = z2 * (z2 * sincosn_zs_[2] + sincosn_zs_[1]) + sincosn_zs_[0];
    double fc = z2 * sincosn_zc_[1] + sincosn_zc_[0];
    exptab_sum_ sg, cg;
    sincosn_(iq, &sg, &cg);
    double r = cg.hi * (z * fs) + (sg.lo + sg.hi * (z2 * fc));
    double lb = (r - sinpi_err_) + sg.hi;
    double ub = (r + sinpi_err_) + sg.hi;

    if (lb == ub)
        return lb;

    double mid;
    if (sinpi_mid_(x, &mid))
        return mid;

    return sinpi_accurate_(x);
}

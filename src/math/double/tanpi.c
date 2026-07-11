/* C23 tanpi: tan(pi*x), correctly rounded.
 *
 * Port of metallic-rs `tanpi` (src/f64_/trig.rs), itself mirroring
 * CORE-MATH's cr_tanpi: the signed-mantissa reduction folds the period and
 * the half-period tangent reflection into a 1/64 grid (iq in 0..=32,
 * tanpitab.h), the exact fixed-point residual takes a short odd tangent
 * polynomial, and tanpi_reconstruct_ finishes through the angle-addition
 * quotient -- -1/t in the pole-adjacent cell.  The double-double fast leg
 * is already accurate-tier quality (eps ~ 2^-104 relative, growing with
 * tan^2), so straddles go directly to the 128-bit dint sine/cosine ratio,
 * which replaces CORE-MATH's exception database outright. */

#include "kernel/tanpitab.h"
#include "kernel/trigdint.h"

/* (tan(pi*x) - pi*x)/x^3 Taylor coefficients for the small band:
 * pi^3/3, 2pi^5/15, 17pi^7/315. */
static const double tanpi_small_c_[3] = {
    0x1.4abbce625be53p+3, 0x1.466bc6775aae2p+5, 0x1.45fff9b48e95ep+7,
};

/* Signed zero/pole returns for on-grid arguments: jq counts half-quadrants,
 * odd ones are poles (+-inf by the side the pole is entered from), even
 * ones zeros (sign by the period-1 half jq & 2 against the argument sign). */
static double tanpi_grid_special_(int64_t jq, int64_t sgn)
{
    if (jq & 1)
        return jq & 2 ? -INFINITY : INFINITY;

    return (jq ^ sgn) & 2 ? -0.0 : 0.0;
}

/* Final tier: the 128-bit dint sine/cosine ratio at the exact reduced angle
 * -- tan(pi*r) in even half-quadrants, -cot(pi*r) in odd ones.  The result
 * is normal (the residual keeps |r| >= 2^-53-class, so |tan| stays within
 * [~2^-52, ~2^53]), which dint_to_f64_ requires. */
static double tanpi_accurate_(double x)
{
    double q = rint(2.0 * x);
    dint_t u = dint_from_f64_(2.0 * x - q);
    dint_t theta = dint_mul_(&u, &PIO2_DINT); /* pi(x - q/2) in [-pi/4, pi/4] */
    dint_t s, c;
    sin_cos_dint_(&theta, &s, &c);
    dint_t w;

    if ((int64_t)q & 1) {
        dint_t r = recip_dint_(&s);
        w = neg_dint_(dint_mul_(&c, &r));
    } else {
        dint_t r = recip_dint_(&c);
        w = dint_mul_(&s, &r);
    }

    return dint_to_f64_(&w);
}

double tanpi(double x)
{
    uint64_t ix = reinterpret(uint64_t, x);
    uint64_t ax = ix & (~0ULL >> 1);

    /* |x| < 2^-12: pi*x with an odd cubic correction. */
    if (ax < 0x3f3ULL << 52) {
        if (ax == 0)
            return x; /* +-0 keeps its sign */

        if (ax < 0x3caULL << 52) {
            /* |x| < 2^-53: tan(pi*x) rounds as the dd product pi*x.  Cold
             * band: the true fma() keeps the cross term one rounding, as in
             * the Rust dd multiply this reproduces. */
            if (ax < 0x06bULL << 52) {
                /* |x| < 2^-916: form the product at a 2^(1022-e) scale and
                 * fold down so the subnormal rounding happens once
                 * (CORE-MATH's fold). */
                int64_t e = (int64_t)(ax >> 52);
                double sc = reinterpret(double, (uint64_t)(2045 - e) << 52);
                double isc = reinterpret(double, (uint64_t)(1 + e) << 52);
                double xs = x * sc;
                exptab_sum_ t = exptab_prod_(tanpi_pi_hi_, xs);
                t.lo = fma(tanpi_pi_lo_, xs, t.lo);
                double res = t.hi * isc;

                if (fabs(res) < 0x1p-1022) {
                    double o = copysign(0x1p-1022, x);
                    double v0h = res * sc;
                    double tl = t.lo + (t.hi - v0h);
                    double v0b = (o + res) * sc + tl;
                    return v0b * isc - o;
                }

                /* Normal result: round in the scaled domain (the 2^-e scale
                 * is exact), gated like the band below. */
                double eps = fabs(t.hi) * 0x1p-102;
                double lb = t.hi + (t.lo - eps);
                double ub = t.hi + (t.lo + eps);

                if (lb == ub)
                    return lb * isc;

                return tanpi_accurate_(x);
            }

            exptab_sum_ t = exptab_prod_(tanpi_pi_hi_, x);
            t.lo = fma(tanpi_pi_lo_, x, t.lo);
            double eps = fabs(t.hi) * 0x1p-102;
            double lb = t.hi + (t.lo - eps);
            double ub = t.hi + (t.lo + eps);

            if (lb == ub)
                return lb;

            return tanpi_accurate_(x);
        }

        /* 2^-53 <= |x| < 2^-12: pi*x plus the odd tangent series. */
        double x2 = x * x;
        double x3 = x * x2;
        double f = x3 * (tanpi_small_c_[0] + x2 * (tanpi_small_c_[1] + x2 * tanpi_small_c_[2]));
        exptab_sum_ p = exptab_prod_(tanpi_pi_hi_, x);
        p.lo += tanpi_pi_lo_ * x;
        exptab_sum_ s = exptab_fast2sum_(p.hi, f);
        exptab_sum_ t = { s.hi, s.lo + p.lo };
        double eps = x * (x2 * 0x1.54p-47 + 0x1p-101);
        double lb = t.hi + (t.lo - eps);
        double ub = t.hi + (t.lo + eps);

        if (lb == ub)
            return lb;

        return tanpi_accurate_(x);
    }

    /* |x| >= 2^46: every value is a multiple of 1/64 -- poles, zeros, +-1,
     * and the grid table are the whole story.  +-inf/NaN reject here too. */
    if (ax >= 0x42dULL << 52) {
        if (ax >= 0x7ffULL << 52)
            return ax > 0x7ffULL << 52 ? x + x : (x - x) / (x - x);

        int64_t e = (int64_t)(ax >> 52);
        int64_t s = e - 1069;

        if (s > 6)
            return copysign(0.0, x); /* |x| >= 2^53: all even integers */

        int64_t sgn = (int64_t)ix >> 63;
        int64_t m = (int64_t)ax; /* only the low mantissa bits survive the mask */
        int64_t iq = (int64_t)(((uint64_t)((m ^ sgn) - sgn) << s) & 127);

        if (!(iq & 31))
            return tanpi_grid_special_(iq >> 5, sgn);

        if (iq & 32) {
            int64_t i = 32 - (iq & 31);
            return -tanpi_t_[i][0] - tanpi_t_[i][1];
        }

        return tanpi_t_[iq & 31][0] + tanpi_t_[iq & 31][1];
    }

    /* Main band 2^-12 <= |x| < 2^46 (e in [1011, 1068]): fold the period and
     * the half-period tangent reflection into the 1/64 grid. */
    int64_t e = (int64_t)(ax >> 52);
    int64_t s = 1068 - e;  /* 0..=57 */
    int64_t s1 = e - 1011; /* 0..=57 */
    int64_t m = (int64_t)((ax & (~0ULL >> 12)) | 1ULL << 52);
    int64_t ms = (int64_t)((uint64_t)m << s1) >> 63; /* the 1/2-weight bit of x */
    int64_t sgn = (int64_t)ix >> 63;
    int64_t iq = (int64_t)((((uint64_t)((m ^ ms) >> s) & 63) + 1) >> 1); /* 0..=32 */
    ms ^= sgn;
    int64_t sm = (m ^ sgn) - sgn;
    int64_t k = (int64_t)((uint64_t)sm << (e - 1005)); /* residual <= 2^-7 bits */
    double z = (double)k;

    if ((uint64_t)k << 1 == 0) {
        /* x is a multiple of 2^-8: poles, zeros, and +-1/4-type exact +-1. */
        if (k == 0) {
            if (!(iq & 31))
                return tanpi_grid_special_(sm >> (s + 6), sgn);

            uint64_t kq = (uint64_t)m << s1 >> 58;

            if (kq == 0x10)
                return copysign(1.0, x); /* |x| = 1/4 mod 1 */

            if (kq == 0x30)
                return -copysign(1.0, x); /* |x| = 3/4 mod 1 */
        }

        z *= copysign(1.0, x); /* k == INT64_MIN carries the wrong sign */
    }

    double z2 = z * z;
    double z4 = z2 * z2;
    double z3 = z * z2;
    double f = z3 * ((tanpi_zc_[0] + z2 * tanpi_zc_[1])
        + z4 * ((tanpi_zc_[2] + z2 * tanpi_zc_[3]) + z4 * tanpi_zc_[4]));
    double eps = z3 * 0x1p-256 + copysign(0x1p-103, z);
    exptab_sum_ p = exptab_prod_(tanpi_ph_hi_, z);
    p.lo += tanpi_ph_lo_ * z;
    exptab_sum_ sum = exptab_fast2sum_(p.hi, f);
    exptab_sum_ t = { sum.hi, sum.lo + p.lo };
    t = tanpi_reconstruct_(t, iq, ms != 0);
    eps = 1.25 * (eps * (t.hi * t.hi) + eps);
    double lb = t.hi + (t.lo - eps);
    double ub = t.hi + (t.lo + eps);

    if (lb == ub)
        return lb;

    return tanpi_accurate_(x);
}

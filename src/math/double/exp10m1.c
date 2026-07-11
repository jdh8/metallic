/* C23 exp10m1: 10^x - 1, correctly rounded.
 *
 * Port of metallic-rs `exp10m1` (src/f64_/exp.rs) -- exp2m1's architecture
 * with exp10's base-10 reduction: the mantissa-space -1 over the two-level
 * fold, the result-anchored small band at |x| <= 0.0625 (plain leg added in
 * this port, then double-double, then the 128-bit dint tier that replaces
 * CORE-MATH's 145-entry exception table), and a 10-entry hard-case database
 * behind the ~2^-105 accurate pipeline. */

#include "kernel/exp2lvl.h"
#include "kernel/exp2lvlacc.h"
#include "kernel/dint.h"
#include <math.h>
#include <stdint.h>

/* (10^x - 1 - x*ln10)/x^2 Taylor coefficients ln10^(k+2)/(k+2)!, low degree
 * first, as double-doubles (metallic-rs EXP10M1_U_COEFFS). */
static const double exp10m1_u_[19][2] = {
    { 0x1.53524c73cea69p+1, -0x1.e2bfab318d695p-53 },
    { 0x1.0470591de2ca4p+1, 0x1.823527cec106ap-53 },
    { 0x1.2bd7609fd98c4p+0, 0x1.31ea51f65ed9fp-54 },
    { 0x1.1429ffd1d4d76p-1, 0x1.1171950896416p-56 },
    { 0x1.a7ed70847c8b6p-3, -0x1.4260c232c8c25p-58 },
    { 0x1.16e4dfc333a87p-4, -0x1.33e9f4455ab4ep-58 },
    { 0x1.4116b05fdaa5dp-6, -0x1.07bba8ef04ca0p-61 },
    { 0x1.4897c45d93d42p-8, -0x1.b3ee92cb29e68p-65 },
    { 0x1.2ea52b2d182afp-10, 0x1.9d3293774eb8cp-64 },
    { 0x1.facfd5d909d64p-13, -0x1.6ef0c20f4f530p-71 },
    { 0x1.84fe12df80be4p-15, 0x1.ab36bd58540bfp-71 },
    { 0x1.1398ad2c41708p-17, 0x1.fb35c95b48388p-71 },
    { 0x1.6a9e984b8a6bfp-20, 0x1.8137e87023d0dp-74 },
    { 0x1.bd502ef78dc59p-23, -0x1.60ee4150eab82p-77 },
    { 0x1.0057c7dff74a8p-25, -0x1.30cc2693f60f4p-79 },
    { 0x1.15c3eca4c219dp-28, -0x1.a258ec237e79dp-82 },
    { 0x1.1c41cc6c71b2ap-31, -0x1.4d42e8e884a91p-88 },
    { 0x1.1397030d51cf4p-34, 0x1.62ffa9ac5c9bfp-89 },
    { 0x1.fba7c444fea83p-38, 0x1.43c47d16a96b8p-92 },
};

/* ln10 as a double-double. */
static const double exp10m1_ln10_hi_ = 0x1.26bb1bbb55516p+1;
static const double exp10m1_ln10_lo_ = -0x1.f48ad494ea3e9p-53;

/* ln10 as a 128-bit dint. */
static const dint_t exp10m1_ln10_dint_ = { 0, 1,
    ((unsigned __int128)0x935d8dddaaa8ac16ULL << 64) | 0xea56d62b82d30a29ULL };

/* Inputs whose 10^x - 1 lies within ~2^-105 of a double midpoint:
 * (input_bits, result_bits), sorted. */
static const uint64_t exp10m1_hard_[10][2] = {
    { 0x3FB76BD8057C5E82, 0x3FCE03EE2E457A0B },
    { 0x3FB9A4FA3D4E586E, 0x3FD09A7B16F688CD },
    { 0x3FBBA4D6B3E1398D, 0x3FD210EE0144E997 },
    { 0x3FDC3E364DA53C4C, 0x3FFC3310C87526D5 },
    { 0x4012D5494EB1DD13, 0x40E8F169A48A6A87 },
    { 0x40658C1635F834D6, 0x63B8B2DCB245522F },
    { 0x406CDE37694F4D10, 0x6FE2210FD5A164B7 },
    { 0xBFB7510CD4059D09, 0xBFC8375A0EFABD75 },
    { 0xC01D3C1633A449A3, 0xBFEFFFFFE5A00421 },
    { 0xC024657D88B39382, 0xBFEFFFFFFFF74AEB },
};

/* Deep tier for |x| <= 0.0625: x*ln10 + c in the 128-bit dint. */
static double exp10m1_deep_(double x, double c_hi, double c_lo)
{
    dint_t xd = dint_from_f64_(x);
    dint_t w = dint_mul_(&xd, &exp10m1_ln10_dint_);

    if (c_hi != 0.0) {
        dint_t t = dint_from_f64_(c_hi);
        w = dint_add_(&w, &t);
    }

    if (c_lo != 0.0) {
        dint_t t = dint_from_f64_(c_lo);
        w = dint_add_(&w, &t);
    }

    return dint_to_f64_general_(&w);
}

/* Correctly-rounded 10^x - 1 for |x| <= 0.0625: exp2m1's result-anchored
 * small band with the base-10 series. */
static double exp10m1_small_(double x)
{
    if (fabs(x) < 0x1p-900)
        return exp10m1_deep_(x, 0.0, 0.0);

    /* Plain-double fast leg: 10^x - 1 = x*ln10 + c, c = x^2*U(x) evaluated
     * in plain double (degree 10: truncation ~2^-57 relative to U at the
     * 0.0625 band edge).  Same budget shape as exp2m1's leg, with the
     * base-10 c/anchor ratio |c|/|x ln10| <= 1.15*0.0625/2.30 ~ 0.031:
     * eps = xx*2^-47 covers the ln10-scaled Horner error with margin, and
     * the |ph|*2^-102 floor covers the anchor's tail for tiny |x|. */
    {
        exptab_sum_ p = exptab_prod_(x, exp10m1_ln10_hi_);
        double xx = x * x;
        double u = exp10m1_u_[10][0];

        for (int k = 9; k >= 0; --k)
            u = u * x + exp10m1_u_[k][0];

        double c = xx * u;
        double low = (p.lo + x * exp10m1_ln10_lo_) + c;
        double eps = xx * 0x1p-47 + fabs(p.hi) * 0x1p-102;
        double left = p.hi + (low - eps);
        double right = p.hi + (low + eps);

        if (left == right)
            return left;
    }

    exptab_sum_ u = { exp10m1_u_[18][0], exp10m1_u_[18][1] };

    for (int k = 17; k >= 0; --k)
        u = exptab_muladd_(u, x, exp10m1_u_[k][0], exp10m1_u_[k][1]);

    exptab_sum_ c = exptab_mul_(exptab_prod_(x, x), u);
    exptab_sum_ w = exptab_prod_(x, exp10m1_ln10_hi_);
    w.lo += x * exp10m1_ln10_lo_;

    exptab_sum_ s = exptab_fast2sum_(w.hi, c.hi);
    w = (exptab_sum_){ s.hi, s.lo + (w.lo + c.lo) };

    double eps = fabs(w.hi) * 0x1p-100;
    double left = w.hi + (w.lo - eps);
    double right = w.hi + (w.lo + eps);

    if (left == right)
        return left;

    return exp10m1_deep_(x, c.hi, c.lo);
}

/* Accurate main band: exp10's base-10 double-double reduction lifted to
 * base e, through the shared mantissa with the -1 in mantissa space. */
static double exp10m1_general_(double x)
{
    uint64_t key = reinterpret(uint64_t, x);

    for (int i = 0; i < 10; ++i)
        if (exp10m1_hard_[i][0] == key)
            return reinterpret(double, exp10m1_hard_[i][1]);

    double tf = rint(x * exp2lvl_n_log2_10_);
    int64_t t = (int64_t)tf;
    double dx0 = x - tf * exp2lvl_log10_2_hi_; /* exact (Sterbenz) */
    double dxl = tf * exp2lvl_log10_2_mid_;
    double dxll = fma(tf, exp2lvl_log10_2_lo_, fma(tf, exp2lvl_log10_2_mid_, -dxl));
    double dh = dx0 + dxl;
    exptab_sum_ delta = { dh, ((dx0 - dh) + dxl) + dxll };
    exptab_sum_ dx = exp2lvlacc_mul_(
        (exptab_sum_){ 2.302585092994046, -2.1707562233822494e-16 }, delta);

    int64_t ie;
    exptab_sum_ mant = exp2lvlacc_mantissa_(t, dx, &ie);

    double off = reinterpret(double, (uint64_t)(2048 + 1023 - ie) << 52);
    exptab_sum_ s = ie < 53 ? exptab_fast2sum_(off, mant.hi)
                            : exptab_fast2sum_(mant.hi, off);
    double fh = exptab_fast2sum_(s.hi, mant.lo + s.lo).hi;

    return shift_(fh, ie);
}

double exp10m1(double x)
{
    if (x != x || x == 0)
        return x; /* NaN propagates; +-0 keeps its sign */

    if (x >= 0x1.34413509f79ffp+8)
        return INFINITY;

    /* 10^x <= 2^-54: rounds to -1 (ties-to-even at the threshold). */
    if (x <= -0x1.041704c068ef0p+4)
        return -1.0;

    if ((reinterpret(uint64_t, x) & (~0ULL >> 1)) <= 0x3FB0000000000000)
        return exp10m1_small_(x);

    double scaled = rint(x * exp2lvl_n_log2_10_);
    int64_t t = (int64_t)scaled;
    double dh = x - scaled * exp2lvl_log10_2_hi_; /* exact (Sterbenz) */
    double delta = dh + scaled * exp2lvl_log10_2_mid_;

    int64_t q;
    exptab_sum_ f = exp2lvl_fold2_(t, delta, exp2lvl_p10_, &q);

    double off = reinterpret(double, (uint64_t)(2048 + 1023 - q) << 52);
    exptab_sum_ s = q < 53 ? exptab_fast2sum_(off, f.hi)
                  : q < 75 ? exptab_fast2sum_(f.hi, off)
                  : (exptab_sum_){ f.hi, 0 };

    double low = f.lo + s.lo;
    double left = s.hi + (low - EXP2LVL_EPS_);
    double right = s.hi + (low + EXP2LVL_EPS_);

    if (left == right)
        return shift_(left, q);

    return exp10m1_general_(x);
}

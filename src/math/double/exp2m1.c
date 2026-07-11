/* C23 exp2m1: 2^x - 1, correctly rounded.
 *
 * Port of metallic-rs `exp2m1` (src/f64_/exp.rs).  The main band |x| >= 1/4
 * rides exp2's exact base-2 reduction through the lean two-level fold with
 * the -1 taken in mantissa space (expm1's off-window); straddles fall to the
 * ~2^-105 exp2lvlacc pipeline with the same mantissa-space subtraction, and
 * a 3-entry hard-case database resolves what that cannot.  The small band
 * |x| < 1/4 is result-anchored x*ln2 + x^2*U(x): a plain-double fast leg
 * (this port's addition -- metallic-rs runs the double-double Horner every
 * call), then the double-double tier, then a 128-bit dint tier that replaces
 * CORE-MATH's 56-entry exception table outright and owns every subnormal
 * result. */

#include "kernel/exp2lvl.h"
#include "kernel/exp2lvlacc.h"
#include "kernel/dint.h"
#include <math.h>
#include <stdint.h>

/* (2^x - 1 - x*ln2)/x^2 Taylor coefficients ln2^(k+2)/(k+2)!, low degree
 * first, as double-doubles (metallic-rs EXP2M1_U_COEFFS). */
static const double exp2m1_u_[19][2] = {
    { 0x1.ebfbdff82c58fp-3, -0x1.5e43a53e44da3p-57 },
    { 0x1.c6b08d704a0c0p-5, -0x1.d331627513351p-59 },
    { 0x1.3b2ab6fba4e77p-7, 0x1.4e65df05a9f75p-62 },
    { 0x1.5d87fe78a6731p-10, 0x1.0717f69a514bfp-66 },
    { 0x1.430912f86c787p-13, 0x1.bd2c2a261ac8dp-67 },
    { 0x1.ffcbfc588b0c7p-17, -0x1.e53ab8cde09c6p-71 },
    { 0x1.62c0223a5c824p-20, -0x1.3800cfc92c41ep-79 },
    { 0x1.b5253d395e7c4p-24, -0x1.2dac78d2d8038p-79 },
    { 0x1.e4cf5158b8ecap-28, -0x1.204bc4d5a312dp-85 },
    { 0x1.e8cac7351bb25p-32, -0x1.f8543350dc6f6p-87 },
    { 0x1.c3bd650fc2986p-36, -0x1.d4a9781e85d12p-92 },
    { 0x1.816193166d0f9p-40, 0x1.8a06b0c03dc6cp-94 },
    { 0x1.314964d5878a9p-44, 0x1.cfc50d89572bbp-98 },
    { 0x1.c36e843b04022p-49, -0x1.984ea80e5635bp-105 },
    { 0x1.38e89ae79f8b4p-53, -0x1.b71c06c4d9c88p-107 },
    { 0x1.98444b41c25a8p-58, -0x1.54f13bff8403fp-113 },
    { 0x1.f7176bdb43696p-63, -0x1.4668963b45e1bp-118 },
    { 0x1.25a7ecb835c65p-67, -0x1.28fd4aa6d42acp-122 },
    { 0x1.45acc4b505136p-72, -0x1.ec76c83fea1e3p-126 },
};

/* ln2 as a double-double. */
static const double exp2m1_ln2_hi_ = 0x1.62e42fefa39efp-1;
static const double exp2m1_ln2_lo_ = 0x1.abc9e3b39803fp-56;

/* Inputs whose 2^x - 1 lies within ~2^-105 of a double midpoint, closer than
 * the accurate path resolves: (input_bits, result_bits), sorted. */
static const uint64_t exp2m1_hard_[3][2] = {
    { 0x3FE1E5F48D8BA05A, 0x3FDE4F0E78AFFDCD },
    { 0xBFD5EA0338C1D1F1, 0xBFCB0B1993050635 },
    { 0xC01558CF280FDE2A, 0xBFEF354A4865F051 },
};

/* Deep tier for |x| < 1/4: x*ln2 + c carried in the 128-bit dint (the
 * double-double correction c = x^2*U(x) enters limb-wise), rounded by the
 * subnormal-safe general finisher. */
static double exp2m1_deep_(double x, double c_hi, double c_lo)
{
    dint_t xd = dint_from_f64_(x);
    dint_t w = dint_mul_(&xd, &dint_log2_);

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

/* Correctly-rounded 2^x - 1 for |x| < 1/4: a plain-double fast leg, then the
 * double-double x*ln2 + x^2*U(x) under a relative gate, with straddles, the
 * deep tail, and every subnormal result resolved by the dint tier. */
static double exp2m1_small_(double x)
{
    /* Below 2^-900 the gates' widths themselves would go subnormal (and the
     * results eventually do); the 128-bit tier owns the rounding outright. */
    if (fabs(x) < 0x1p-900)
        return exp2m1_deep_(x, 0.0, 0.0);

    /* Result-anchored plain leg: exp2m1(x) = x*ln2 + c with the correction
     * c = x^2*U(x) > 0 evaluated in plain double (degree 10: the truncation
     * is ~2^-59 relative to U at |x| = 1/4).  The anchor x*ln2 is a Dekker
     * product plus the x*ln2_lo fold, exact to ~2^-106 relative.  Error
     * rides c -- Horner ~2^-50*c, xx and the folds ~2^-52*c -- so
     * eps = xx*2^-49 covers it with >= 4x margin; the |ph|*2^-102 floor
     * covers the anchor's own tail for tiny |x| where c underflows. */
    {
        exptab_sum_ p = exptab_prod_(x, exp2m1_ln2_hi_);
        double xx = x * x;
        double u = exp2m1_u_[10][0];

        for (int k = 9; k >= 0; --k)
            u = u * x + exp2m1_u_[k][0];

        double c = xx * u;
        double low = (p.lo + x * exp2m1_ln2_lo_) + c;
        double eps = xx * 0x1p-49 + fabs(p.hi) * 0x1p-102;
        double left = p.hi + (low - eps);
        double right = p.hi + (low + eps);

        if (left == right)
            return left;
    }

    /* Double-double tier: 19-term Horner of U, relative gate 2^-100 (the dd
     * slope truncates at ~2^-107 and the Horner roundings sit near 2^-104). */
    exptab_sum_ u = { exp2m1_u_[18][0], exp2m1_u_[18][1] };

    for (int k = 17; k >= 0; --k)
        u = exptab_muladd_(u, x, exp2m1_u_[k][0], exp2m1_u_[k][1]);

    exptab_sum_ c = exptab_mul_(exptab_prod_(x, x), u);
    exptab_sum_ w = exptab_prod_(x, exp2m1_ln2_hi_);
    w.lo += x * exp2m1_ln2_lo_;

    /* w + c, Fast2Sum on the highs (|c| <= |w|*|x|*0.35, ordered). */
    exptab_sum_ s = exptab_fast2sum_(w.hi, c.hi);
    w = (exptab_sum_){ s.hi, s.lo + (w.lo + c.lo) };

    double eps = fabs(w.hi) * 0x1p-100;
    double left = w.hi + (w.lo - eps);
    double right = w.hi + (w.lo + eps);

    if (left == right)
        return left;

    return exp2m1_deep_(x, c.hi, c.lo);
}

/* Accurate main band: the exp2lvlacc mantissa with the -1 in mantissa space.
 * |x| >= 1/4 keeps ie in [-54, 1023] and bounds the cancellation to ~3 bits. */
static double exp2m1_general_(double x)
{
    uint64_t key = reinterpret(uint64_t, x);

    for (int i = 0; i < 3; ++i)
        if (exp2m1_hard_[i][0] == key)
            return reinterpret(double, exp2m1_hard_[i][1]);

    double tf = rint(x * 4096.0);
    int64_t t = (int64_t)tf;
    double sigma = x * 4096.0 - tf; /* exact: 4096 is a power of two */

    /* dx = sigma * ln2/4096 as a double-double: ln2/4096 = l2h - l2l - l2ll,
     * with -sigma*l2l landing inside the high word (Fast2Sum) and the fine
     * residuals gathered in the low word (true fma, cold path). */
    exptab_sum_ p = exptab_prod_(sigma, exp2lvlacc_l2h_);
    exptab_sum_ q = exptab_prod_(sigma, -exp2lvlacc_l2l_);
    double hi = p.hi + q.hi;
    double e = (p.hi - hi) + q.hi;
    exptab_sum_ dx = { hi, e + (p.lo + fma(sigma, -exp2lvlacc_l2ll_, q.lo)) };

    int64_t ie;
    exptab_sum_ mant = exp2lvlacc_mantissa_(t, dx, &ie);

    double off = reinterpret(double, (uint64_t)(2048 + 1023 - ie) << 52);
    exptab_sum_ s = ie < 53 ? exptab_fast2sum_(off, mant.hi)
                            : exptab_fast2sum_(mant.hi, off);
    double fh = exptab_fast2sum_(s.hi, mant.lo + s.lo).hi;

    return shift_(fh, ie);
}

double exp2m1(double x)
{
    if (x != x || x == 0)
        return x; /* NaN propagates; +-0 keeps its sign */

    if (x >= 1024.0)
        return INFINITY;

    /* 2^x <= 2^-54: the result rounds to -1 (ties-to-even at x = -54). */
    if (x <= -54.0)
        return -1.0;

    if ((reinterpret(uint64_t, x) & (~0ULL >> 1)) < 0x3FD0000000000000)
        return exp2m1_small_(x);

    /* Main band: 2^x = (th + fl)*2^q by the exact base-2 reduction, then
     * the -1 in mantissa space exactly as in expm1. */
    double scaled4 = rint(x * 4096.0);
    int64_t t = (int64_t)scaled4;
    double sigma4 = x * 4096.0 - scaled4; /* exact */
    double dx = sigma4 * exp2lvl_ln2_over_n_hi_ + sigma4 * exp2lvl_ln2_over_n_lo_;

    int64_t q;
    exptab_sum_ f = exp2lvl_fold_(t, dx, &q);

    double off = reinterpret(double, (uint64_t)(2048 + 1023 - q) << 52);
    exptab_sum_ s = q < 53 ? exptab_fast2sum_(off, f.hi)
                  : q < 75 ? exptab_fast2sum_(f.hi, off)
                  : (exptab_sum_){ f.hi, 0 };

    double low = f.lo + s.lo;
    double left = s.hi + (low - EXP2LVL_EPS_);
    double right = s.hi + (low + EXP2LVL_EPS_);

    if (left == right)
        return shift_(left, q);

    return exp2m1_general_(x);
}

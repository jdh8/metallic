/* C23 exp10: 10^x, correctly rounded.
 *
 * Port of metallic-rs `exp10` (src/f64_/exp.rs).  The fast leg reduces
 * directly in base 10 -- t = round(4096*x*log2(10)), delta = x - t*log10(2)/
 * 4096 -- and folds 10^delta through the shared two-level grid with ln10
 * baked into the tail polynomial, so no double-double x*ln10 is ever formed.
 * Straddles of the 2^-62 Ziv gate fall to the ~2^-105 double-double pipeline
 * (three-word log10(2)/4096 reduction, then delta*ln10 into the shared
 * base-e finish), whose few remaining near-ties a 13-entry database resolves
 * (each entry independently confirmed by a 200-bit MPFR exp10). */

#include "kernel/exp2lvl.h"
#include "kernel/exp2lvlacc.h"
#include "shift.h"
#include <math.h>
#include <stdint.h>

/* Inputs whose 10^x lies within ~2^-105 of a double midpoint, closer than
 * the accurate path resolves: (input_bits, result_bits), sorted. */
static const uint64_t exp10_hard_[13][2] = {
    { 0x3F57C3DDD23AC8CA, 0x3FF00DB40291E4F5 }, { 0x3F890D7373B3A546, 0x3FF07502EB858F1F },
    { 0x3F97E3C84F2CB9B5, 0x3FF0E20CD9BD42A9 }, { 0x4012D5494EB1DD13, 0x40E8F189A48A6A87 },
    { 0x40658C1635F834D6, 0x63B8B2DCB245522F }, { 0x406CDE37694F4D10, 0x6FE2210FD5A164B7 },
    { 0xBF0485426A688467, 0x3FEFFF4301DF6B37 }, { 0xBF56EE0B282F77EC, 0x3FEFE5A4BD73731B },
    { 0xBF79EF920BED31B3, 0x3FEF896D78214048 }, { 0xBF7F6F96F005FD47, 0x3FEF708106038D25 },
    { 0xBFA485B2AC5F7EC9, 0x3FED2DC5E913A8FB }, { 0xC034CD4AF2FCA2B4, 0x3B9DCE2DFAAF67C9 },
    { 0xC06E095699D01895, 0x0E0B3E5F2B6612DF },
};

static double exp10_accurate_(double x)
{
    uint64_t key = reinterpret(uint64_t, x);
    int lo = 0, hi = 13;

    while (lo < hi) {
        int mid = (lo + hi) >> 1;

        if (exp10_hard_[mid][0] < key)
            lo = mid + 1;
        else
            hi = mid;
    }

    if (lo < 13 && exp10_hard_[lo][0] == key)
        return reinterpret(double, exp10_hard_[lo][1]);

    double tf = rint(x * exp2lvl_n_log2_10_);
    int64_t t = (int64_t)tf;

    /* delta = x - t*log10(2)/4096 as a double-double: tf*hi is exact and
     * cancels against x (Sterbenz); mid/lo carry the rest (true fma, like
     * the rest of this cold pipeline). */
    double dx0 = x - tf * exp2lvl_log10_2_hi_;
    double dxl = tf * exp2lvl_log10_2_mid_;
    double dxll = fma(tf, exp2lvl_log10_2_lo_, fma(tf, exp2lvl_log10_2_mid_, -dxl));
    double dh = dx0 + dxl;
    exptab_sum_ delta = { dh, ((dx0 - dh) + dxl) + dxll };

    /* Lift to base e: dx = delta*ln10, so 10^delta = exp(dx) and the shared
     * base-e finish applies; the product's rounding rides far below the
     * finish's ~2^-105 budget. */
    exptab_sum_ dx = exp2lvlacc_mul_(
        (exptab_sum_){ 2.302585092994046, -2.1707562233822494e-16 }, delta);

    return exp2lvlacc_finish_(t, dx);
}

double exp10(double x)
{
    if (x != x)
        return x;

    /* Overflow threshold, exact to the ulp: exp10 is finite at
     * 0x1.34413509f79fep+8 (~DBL_MAX) and overflows at the next input up.
     * The rounded decimal 308.2547155559... would clip the last finite
     * input, so gate on the exact first overflowing bit pattern. */
    if (x >= 0x1.34413509f79ffp+8)
        return INFINITY;

    /* 10^x < 2^-1075: rounds to +0 (the boundary maps into the subnormals,
     * which the accurate path's subnormal-safe rounding handles). */
    if (x <= -323.6072453387798)
        return 0;

    double scaled = rint(x * exp2lvl_n_log2_10_);
    int64_t t = (int64_t)scaled;

    /* delta = (x - scaled*hi) + scaled*mid: the first product is exact and
     * cancels (Sterbenz); the dropped scaled*lo ~ 2^-78 and the plain
     * mul+add's extra rounding ride far below the leg's budget. */
    double dh = x - scaled * exp2lvl_log10_2_hi_;
    double delta = dh + scaled * exp2lvl_log10_2_mid_;

    int64_t q;
    exptab_sum_ f = exp2lvl_fold2_(t, delta, exp2lvl_p10_, &q);

    /* q >= -1021 keeps the subnormal transition (where the mantissa's
     * normalization can shift q) on the accurate path. */
    if (q >= -1021) {
        double lo = f.hi + (f.lo - EXP2LVL_EPS_);
        double hi = f.hi + (f.lo + EXP2LVL_EPS_);

        if (lo == hi)
            return shift_(lo, q);
    }

    return exp10_accurate_(x);
}

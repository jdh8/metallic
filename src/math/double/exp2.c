#include "kernel/exptab.h"
#include "kernel/exp2tab.h"
#include "kernel/exp2lvl.h"
#include <math.h>
#include <stdint.h>

double exp2(double x)
{
    if (isnan(x))
        return x;

    if (x >= 1024)
        return INFINITY;

    if (x <= -1075)
        return 0;

    /* |exp2(x) - 1| < |x|·ln2·(1 + 2^-50) < 0.694·2^-54, so exp2(x) lies
     * strictly inside (1-2^-54, 1+2^-53) -- strictly between both rounding
     * midpoints of 1 -- and rounds to 1, as does the computed 1 + x.  The
     * threshold must be tighter than exp's 2^-53: for x ∈ (-2^-53.53, -2^-54)
     * exp2 still rounds to 1 but 1 + x rounds to 1-2^-53 (ln2 < 1 skews the
     * windows).  The nearest exp2_wc_ entry (≈2^-52.5) lies outside. */
    if (fabs(x) < 0x1p-54)
        return 1 + x;

    /* Lean two-level fast leg: exp2(x) = (th + fl)·2^q on the 4096-point
     * grid, with an almost exact base-2 reduction: sigma = 4096·x − scaled is
     * exact (power-of-two scale + Sterbenz), and dx = sigma·(hi + lo) differs
     * from metallic-rs's fma form only by the sigma·lo product rounding
     * ≤ 2^-95, leaving dx within ~2^-67 of sigma·ln2/4096 — the fold's own
     * budget dominates, well inside the 2^-62 gate.  |sigma| ≥ ulp(x)·2^12
     * bounds |fl| ≥ 2^-43.5 for x just below 1024, so `left` cannot round up
     * to 1.0 at q = 1024 and shift_ stays exact; q ≥ -1021 keeps the
     * subnormal boundary on the accurate path below. */
    {
        double scaled = rint(x * 4096.0);
        double sigma = x * 4096.0 - scaled;
        double dx = sigma * exp2lvl_ln2_over_n_hi_ + sigma * exp2lvl_ln2_over_n_lo_;
        int64_t q;
        exptab_sum_ f = exp2lvl_fold_((int64_t)scaled, dx, &q);

        if (q >= -1021) {
            double left = f.hi + (f.lo - EXP2LVL_EPS_);
            double right = f.hi + (f.lo + EXP2LVL_EPS_);

            if (left == right)
                return shift_(left, q);
        }
    }

    /* n = round(N * x), j = n mod N, q = n / N.
     * sigma = x * N - n is exact (N is a power of 2, Sterbenz). */
    double scaled = rint(x * (double)EXPTAB_N);
    int64_t n = (int64_t)scaled;
    int j = (int)(n & (EXPTAB_N - 1));
    int64_t q = n >> 7;

    /* r = sigma * (ln2/N) as a double-double: the argument to exp(r). */
    double sigma = x * (double)EXPTAB_N - scaled;
    exptab_sum_ product = exptab_prod_(sigma, exptab_ln2_over_n_hi_);
    exptab_sum_ r = exptab_fast2sum_(product.hi,
        product.lo + sigma * exptab_ln2_over_n_lo_);

    exptab_sum_ p = exptab_mantissa_(j, &q, r);

    if (q >= -1022) {
        double s = p.hi + p.lo;
        double e = (p.hi - s) + p.lo;

        if (fabs(e) + 0x1p-98 < 0x1p-53)
            return shift_(s, q);

        /* Near a rounding midpoint: look up the precomputed table. */
        double wc = exp2_wc_lookup_(x);
        return wc ? wc : shift_(s, q);
    }

    /* Subnormal path. */
    {
        double wc = exp2_wc_lookup_(x);
        if (wc) return wc;
    }

    int64_t shift = q + 1074;
    double high = shift_(p.hi, shift);
    double low = p.lo ? shift_(p.lo, shift) : 0.0;
    double m0 = rint(high);
    double m = m0 + rint((high - m0) + low);
    return m * 0x1p-1074;
}

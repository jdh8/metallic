#include "kernel/exptab.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Correctly-rounded hypot via Dekker/TwoProduct exact sum of squares.
 *
 * S = a^2 + b^2 is computed exactly as (s_hi + e_hi + e_lo) using Dekker products
 * and TwoSum.  This triple-word representation provides enough precision to decide
 * the final rounding even for the hardest TMD cases.
 *
 * For normal outputs, compare S to m^2 = (rf + ulp/2)^2 = rf^2 + rf*ulp + ulp^2/4
 * using separate hi/lo parts for the gap to avoid losing ulp^2/4 when it is below
 * machine epsilon relative to rf*ulp.
 *
 * For subnormal outputs (ae==0 path only), the subnormal-grid midpoint in the inner
 * domain is compared to S_inner using the exact gap formula. */

/* Decide the correctly-rounded sqrt of S = s_hi + e_hi + e_lo.
 * rf = nearest double to sqrt(S_inner); rs1*rs2 = inv_scale (split for overflow). */
static double decide_(double rf, double s_hi, double e_hi, double e_lo,
                      double rs1, double rs2)
{
    if (isinf(rf))
        return rf * rs1 * rs2;

    exptab_sum_ rf2 = exptab_prod_(rf, rf);

    /* resid = S - rf^2 = (s_hi - rf2.hi) + (e_hi - rf2.lo) + e_lo.
     * TwoSum each subtraction so the rounding errors are captured. */
    exptab_sum_ q1 = exptab_twosum_(s_hi, -rf2.hi);  /* exact (s_hi - rf2.hi) + q1.lo */
    exptab_sum_ q2 = exptab_twosum_(e_hi, -rf2.lo);  /* exact (e_hi - rf2.lo) + q2.lo */
    double d1 = q1.hi;
    double d2 = q2.hi;
    double e_lo2 = e_lo + q1.lo + q2.lo;   /* residual correction terms */
    double res = d1 + d2 + e_lo2;

    double rf_out = rf * rs1 * rs2;
    /* Use the subnormal decision path when the output is subnormal OR when the
     * output is in the range where the inner-domain step might fall below the
     * subnormal ULP 2^{-1074}.  This happens when scale=2^{600} (subnormal inputs,
     * ae==0) and rf_out is near the subnormal/normal boundary.  The condition
     * rs1 == rs2 is a proxy for "we are in the ae==0 path". */
    double rs_sub = reinterpret(double, (uint64_t)723 << 52);  /* 2^{-300} */
    int subnormal = (reinterpret(uint64_t, rf_out) >> 52) == 0 ||
                    ((rs1 == rs_sub) & ((reinterpret(uint64_t, rf_out) >> 52) == 1));

    if (!subnormal) {
        /* Normal output.
         * gap = m^2 - rf^2 = rf*ulp + ulp^2/4 where m = rf + ulp/2. */
        if (res == 0)
            return rf_out;

        double nb = nextafter(rf, res > 0 ? INFINITY : -INFINITY);
        double ulp = nb - rf;
        double gap_hi = rf * ulp;
        double gap_lo = 0.25 * ulp * ulp;

        /* res_m = S - m^2 = d1 + d2 + e_lo - gap_hi - gap_lo.
         *
         * Two orderings, chosen to minimize cancellation:
         * (A) when |d1| >= |d2|: ((d1 - gap_hi) + d2 + e_lo) - gap_lo
         * (B) when |d2| >  |d1|: ((d2 - gap_hi) + d1 + e_lo) - gap_lo
         * Subtracting gap_hi from the dominant term first avoids large
         * cancellation errors in the subsequent additions. */
        double res_m;
        if (d1 * d1 >= d2 * d2)
            res_m = ((d1 - gap_hi) + d2 + e_lo2) - gap_lo;
        else
            res_m = ((d2 - gap_hi) + d1 + e_lo2) - gap_lo;

        if (res_m == 0) {
            return reinterpret(uint64_t, rf_out) & 1 ?
                nextafter(rf_out, res > 0 ? INFINITY : -INFINITY) : rf_out;
        }

        return (res_m > 0) == (res > 0) ? nb * rs1 * rs2 : rf_out;

    }

    /* Subnormal output (only from ae==0 path, scale = 2^600, rs1=rs2=2^{-300}).
     *
     * The two subnormal candidates are rf_out and its neighbour.  In the inner
     * domain, their separation is sub_ulp_inner = 2^{-474}.
     * We recompute the residual against rf_sub_inner = rf_out * 2^{600} (exact),
     * then compare to the gap = sub_ulp_inner/2 * (2*rfsi + sub_ulp_inner/2)
     *                        = rfsi * 2^{-474} + 2^{-950}. */
    double sub_ulp = reinterpret(double, UINT64_C(1));   /* 2^{-1074} */
    double sc = reinterpret(double, (uint64_t)1323 << 52);  /* 2^{300} */
    double rf_sub_inner = rf_out * sc * sc;              /* rf_out * 2^{600} */

    exptab_sum_ rfsi2 = exptab_prod_(rf_sub_inner, rf_sub_inner);
    double res_sub = (s_hi - rfsi2.hi) + ((e_hi + e_lo) - rfsi2.lo);

    double two_n474 = reinterpret(double, (uint64_t)549 << 52);  /* 2^{-474} */
    double two_n950 = reinterpret(double, (uint64_t)73 << 52);   /* 2^{-950} */
    double gap = rf_sub_inner * two_n474 + two_n950;

    if (res_sub > 0) {
        double res_m = res_sub - gap;
        if (res_m == 0)
            return reinterpret(uint64_t, rf_out) & 1 ? rf_out + sub_ulp : rf_out;
        return res_m > 0 ? rf_out + sub_ulp : rf_out;
    }

    if (res_sub < 0) {
        double rf_low = rf_out - sub_ulp;
        double res_m = res_sub + gap;
        if (res_m == 0)
            return reinterpret(uint64_t, rf_low) & 1 ? rf_out : rf_low;
        return res_m > 0 ? rf_out : rf_low;
    }

    return rf_out;
}

static double inner_(double a, double b, double rs1, double rs2)
{
    exptab_sum_ aa = exptab_prod_(a, a);  /* exact a^2 = aa.hi + aa.lo */
    exptab_sum_ bb = exptab_prod_(b, b);  /* exact b^2 = bb.hi + bb.lo */

    /* S = aa.hi + aa.lo + bb.hi + bb.lo exactly.
     * TwoSum(aa.hi, bb.hi) gives (s_hi, s_lo) with s_hi + s_lo = aa.hi + bb.hi.
     * Then e = aa.lo + bb.lo + s.lo; keep as (e_hi, e_lo) via TwoSums. */
    exptab_sum_ s = exptab_twosum_(aa.hi, bb.hi);

    /* t = aa.lo + bb.lo (with compensation) */
    exptab_sum_ t = exptab_twosum_(aa.lo, bb.lo);

    /* e_hi + e_lo = t.hi + t.lo + s.lo */
    exptab_sum_ e_sum = exptab_twosum_(t.hi, s.lo);
    double e_hi = e_sum.hi;
    double e_lo = e_sum.lo + t.lo;

    if (s.hi == 0 && e_hi == 0 && e_lo == 0)
        return 0;

    double rf = sqrt(s.hi + e_hi + e_lo);
    return decide_(rf, s.hi, e_hi, e_lo, rs1, rs2);
}

double hypot(double x, double y)
{
    uint64_t ux = reinterpret(uint64_t, fabs(x));
    uint64_t uy = reinterpret(uint64_t, fabs(y));

    if (ux < uy) {
        uint64_t t = ux; ux = uy; uy = t;
    }

    const uint64_t inf = 0x7FF0000000000000u;

    if (ux >= inf) {
        if (ux == inf || uy == inf)
            return INFINITY;
        return reinterpret(double, ux) + reinterpret(double, uy);
    }

    if (uy == 0)
        return reinterpret(double, ux);

    double a = reinterpret(double, ux);
    double b = reinterpret(double, uy);

    int64_t ae = (int64_t)(ux >> 52);

    if (ae == 0) {
        /* Subnormal inputs: scale up by 2^{300} * 2^{300} = 2^{600}. */
        double sc = reinterpret(double, (uint64_t)1323 << 52);  /* 2^{300} */
        double rs = reinterpret(double, (uint64_t)723 << 52);   /* 2^{-300} */
        return inner_(a * sc * sc, b * sc * sc, rs, rs);
    }

    int64_t p = 1055 - ae;

    if (p == 0)
        return inner_(a, b, 1.0, 1.0);

    int64_t p1 = p >> 1;
    int64_t p2 = p - p1;

    double s1  = reinterpret(double, (uint64_t)(p1 + 1023) << 52);
    double s2  = reinterpret(double, (uint64_t)(p2 + 1023) << 52);
    double rs1 = reinterpret(double, (uint64_t)(-p1 + 1023) << 52);
    double rs2 = reinterpret(double, (uint64_t)(-p2 + 1023) << 52);

    return inner_(a * s1 * s2, b * s1 * s2, rs1, rs2);
}

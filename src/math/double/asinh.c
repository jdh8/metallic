/* Correctly-rounded double asinh.
 *
 * asinh(x) = ln(x + sqrt(x^2 + 1))   [odd function]
 *
 * For |x| < 2^-27: asinh(x) = x (exact, error < x^3/6 < 0.5 ulp).
 * For |x| <= 0.5:  use ln(1 + x + x^2/(1+sqrt(1+x^2))) to avoid cancellation.
 * For 0.5 < |x| < 2^511: asinh(x) = sign(x) * ln(|x| + sqrt(|x|^2+1)).
 *   - Double-double: form arg as a double-double, call logtab_ln_dd_ + correction.
 * For |x| >= 2^511: x^2 overflows; asinh(x) ≈ sign(x)*(ln(2|x|)) = sign(x)*(ln(|x|)+ln2).
 *
 * Fast path uses relative ERR (as in asin.c) to ensure Ziv test triggers correctly.
 * Accurate path: asin_sqrt_dint_ from kernel/asin.h, then dint_ln_.
 */

#include "kernel/asin.h"    /* asin_sqrt_dint_, dint machinery */
#include "kernel/logtab.h"  /* logtab_ln_dd_ */
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Correctly round x + c, where x is an exact f64 and c a double-double
 * correction with |c| <= |x| (so x is the leading term).  Result-anchored
 * finisher (Boldo-Melquiond round-to-odd): x + c.hi and then s.lo + c.lo are
 * each TwoSum-exact, so the exact sticky tail `we` breaks every half-ulp tie by
 * the true sign of the sub-ulp remainder.  Port of metallic-rs round_anchored. */
static double round_anchored_(double x, exptab_sum_ c)
{
    exptab_sum_ s = exptab_twosum_(x, c.hi);
    exptab_sum_ w = exptab_twosum_(s.lo, c.lo);
    uint64_t wb = reinterpret(uint64_t, w.hi);

    if (w.lo != 0.0 && !(wb & 1)) {
        if (signbit(w.lo) == signbit(w.hi))
            wb += 1;   /* step away from zero, toward the sticky tail */
        else
            wb -= 1;
    }

    return s.hi + reinterpret(double, wb);
}

/* Fast-path error bound: relative to r.hi (2 ULP). */
#define ASINH_ERR(r_hi) ((r_hi) * 0x1p-51)

static double asinh_accurate_(double x)
{
    double ax = fabs(x);
    dint_t axd = dint_from_f64_(ax);

    if (ax >= 0x1p511) {
        /* For very large x: asinh(x) ≈ ln(2x) = ln(x) + ln(2).
         * In dint: form 2*x, apply dint_ln_. */
        dint_t two_x = { 0, axd.ex + 1, axd.m };
        dint_t ln_r = dint_ln_(two_x);
        return copysign(dint_to_f64_(&ln_r), x);
    }

    /* General: ln(|x| + sqrt(x^2+1)). */
    dint_t x2 = dint_mul_(&axd, &axd);
    dint_t one = { 0, 0, (unsigned __int128)0x8000000000000000ULL << 64 };
    dint_t x2p1 = dint_add_(&x2, &one);
    dint_t sq = asin_sqrt_dint_(&x2p1);
    dint_t arg = dint_add_(&axd, &sq);
    dint_t ln_r = dint_ln_(arg);

    return copysign(dint_to_f64_(&ln_r), x);
}

/* Accurate leg of the small-|x| series asinh(x) = x + x^3·S(x^2), reached on a
 * Ziv straddle.  S as a double-double Horner table (degree 13, truncation
 * ~2^-117 over |x| < 1/16); the correction c = x^3·S is built in double-double
 * (its ~2^-104 relative error rides the tiny c), then round_anchored adds the
 * exact x and breaks the half-ulp ties.  Port of metallic-rs asinh_small_accurate
 * (used instead of the dint ln path, which loses a ulp for tiny x where the
 * argument x + sqrt(x^2+1) collapses onto its leading 1). */
static double asinh_small_accurate_(double x)
{
    static const double s_dd[14][2] = {
        { -0.16666666666666666,   -9.25185853854297e-18  },
        {  0.075,                  2.7755575615628915e-18 },
        { -0.044642857142857144,   9.912705577010326e-19 },
        {  0.030381944444444444,   3.854941057726238e-19 },
        { -0.022372159090909092,   9.462128050782583e-19 },
        {  0.017352764423076924,  -8.006416042969879e-19 },
        { -0.01396484375,          6.938893903907229e-19 },
        {  0.011551800896139705,   8.163404592832033e-19 },
        { -0.009761609529194078,  -5.478074134663601e-19 },
        {  0.008390335809616815,   4.130293990420969e-19 },
        { -0.0073125258735988454,  3.394024192128536e-19 },
        {  0.006447210311889649,  -3.1225022567582527e-19 },
        { -0.005740037670841924,   1.2849803525754126e-19 },
        {  0.005153309682319905,  -3.888173308223878e-19 },
    };

    double ax = fabs(x);
    exptab_sum_ v = exptab_prod_(ax, ax);   /* x^2 exact */
    exptab_sum_ s = { s_dd[13][0], s_dd[13][1] };

    for (int i = 12; i >= 0; --i)
        s = exptab_add_(exptab_mul_(s, v), (exptab_sum_){ s_dd[i][0], s_dd[i][1] });

    exptab_sum_ c = exptab_mul_(exptab_mul_((exptab_sum_){ ax, 0.0 }, v), s);  /* x^3·S */
    return copysign(round_anchored_(ax, c), x);
}

double asinh(double x)
{
    if (x != x)
        return x;
    if (x == 0 || x == INFINITY || x == -INFINITY)
        return x;

    double ax = fabs(x);

    /* |x| < 2^-27: asinh(x) = x (error < x^3/6 < 0.5 ulp(x)). */
    if (ax < 0x1p-27)
        return x;

    /* Small |x| < 1/16: result-anchored odd series asinh(x) = x + x^3·S(x^2),
     * S(v) = (asinh(x) - x)/x^3 = sum (-1)^n (2n)!/(4^n (n!)^2 (2n+1)) v^n.
     * The general ln(1 + x + ...) path loses relative precision when the result
     * is this small (a tiny ln near 1), so its 2-ulp gate is unsound there —
     * the lone CORE-MATH asinh.wc misses all fall in this band.  Anchoring at
     * the exact x keeps the error on the small x^3·S correction, and this fast
     * leg needs no sqrt/ln so it is also a speedup.  (Port of metallic-rs
     * asinh_small; ASINH_S_FAST degree 8, truncation ~2^-76 over the band.) */
    if (ax < 0.0625) {
        static const double s_fast[9] = {
            -0.16666666666666666,  0.075,                 -0.044642857142857144,
             0.030381944444444444, -0.022372159090909092,  0.017352764423076924,
            -0.01396484375,         0.011551800896139705, -0.009761609529194078,
        };
        double v = ax * ax;
        double x3 = ax * v;
        double s = ((((((((s_fast[8] * v + s_fast[7]) * v + s_fast[6]) * v + s_fast[5]) * v
            + s_fast[4]) * v + s_fast[3]) * v + s_fast[2]) * v + s_fast[1]) * v + s_fast[0]);
        double tail = x3 * s;
        exptab_sum_ hl = exptab_fast2sum_(ax, tail);  /* |ax| >> |tail| */
        double err = 0x1p-49 * x3;
        double lo = hl.hi + (hl.lo - err);
        double hi = hl.hi + (hl.lo + err);
        if (lo == hi)
            return copysign(lo, x);
        return asinh_small_accurate_(x);
    }

    exptab_sum_ r;

    if (ax >= 0x1p511) {
        /* x^2 overflows; asinh(x) ≈ ln(2x) = ln(x) + ln(2).
         * Compute logtab_ln_dd_(ax) and add ln2 as a double-double. */
        r = logtab_ln_dd_(ax);
        r = exptab_add_(r, (exptab_sum_){ logtab_ln2_hi_, logtab_ln2_lo_ });
    } else if (ax >= 64.0) {
        /* Sqrt-free asymptotic: asinh(x) = ln(2x) + f(v), v = 1/x².
         * f(v) = v/4 - 3v²/32 + 5v³/96 - 35v⁴/1024 + 63v⁵/2560.
         * Degree-5 expansion; truncation error < 2⁻⁷² for |x| >= 64.
         * Avoids an expensive sqrt over the [64, 2^511) range. */
        static const double asymp[5] = {
             0.25,                /* 1/4      */
            -0.09375,             /* -3/32    */
             5.0/96.0,            /* 5/96     */
            -0.034179687500000,   /* -35/1024 */
             0.024609375000000,   /* 63/2560  */
        };
        double v = 1.0 / (ax * ax);
        double correction = v * ((((asymp[4]*v + asymp[3])*v + asymp[2])*v + asymp[1])*v + asymp[0]);
        r = logtab_ln_dd_(ax);
        r = exptab_add_(r, (exptab_sum_){ logtab_ln2_hi_, logtab_ln2_lo_ });
        r = exptab_fast2sum_(r.hi, r.lo + correction);
    } else if (ax <= 0.5) {
        /* Stable for small x: arg = 1 + x + x^2/(1+sqrt(1+x^2)).
         * The result is close to 1, so we use logtab_ln_dd_ directly.
         * Correction for the double-double argument: we compute arg_hi = 1+ax+s/(1+sq),
         * arg_lo captures rounding in the argument. */
        double s   = ax * ax;
        double sq  = sqrt(1.0 + s);
        double frac = s / (1.0 + sq);
        /* arg = 1 + ax + frac, form carefully */
        double t_hi = 1.0 + ax;
        double t_lo = (ax - (t_hi - 1.0));  /* fast2sum residual */
        double arg_hi = t_hi + frac;
        double arg_lo = t_lo + (frac - (arg_hi - t_hi));  /* two-sum residual */
        r = logtab_ln_dd_(arg_hi);
        r.lo += arg_lo / arg_hi;
    } else {
        /* General: ln(ax + sqrt(ax^2+1)).
         * Form the argument as a double-double to avoid losing precision. */
        double s  = ax * ax;
        double sq = sqrt(s + 1.0);
        /* arg = ax + sq; correct the rounding in sqrt. */
        double arg_hi = ax + sq;
        /* sqrt correction: sq_true ≈ sq + (s+1 - sq^2)/(2*sq) */
        double sq_lo = (s + 1.0 - sq * sq) / (2.0 * sq);
        /* arg = ax + sq + sq_lo; arg_hi = ax + sq; arg_lo = sq_lo + (ax + sq - arg_hi) */
        double arg_lo = sq_lo + (ax - (arg_hi - sq));
        r = logtab_ln_dd_(arg_hi);
        r.lo += arg_lo / arg_hi;
    }

    double err   = ASINH_ERR(r.hi);
    double left  = r.hi + (r.lo - err);
    double right = r.hi + (r.lo + err);

    if (left != right)
        return asinh_accurate_(x);

    return copysign(left, x);
}

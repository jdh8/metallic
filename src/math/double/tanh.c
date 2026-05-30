#include "kernel/sinhtab.h"
#include "divs.h"
#include <math.h>
#include <stdint.h>

/* Polynomial for tanh(x)/x − 1 evaluated at x² ∈ [0, (0.2554...)²].
 * Coefficients from the original tanh.c. */
static double kernel_(double x)
{
    const double c[] = {
        3.33333333333333325270e-1,
       -2.22222222222133128770e-2,
        2.11640211480687751936e-3,
       -2.11640107146572980826e-4,
        2.13747028936886696782e-5,
       -2.12203166597434505056e-6
    };

    return x * (((((c[5] * x + c[4]) * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0]);
}

double tanh(double x)
{
    double s = fabs(x);

    /* NaN: propagate before the int cast in the reduction causes UB. */
    if (isnan(x))
        return x;

    /* For |x| >= 20, tanh(x) rounds to ±1 (e^40 > 2^53). */
    if (s >= 20.0)
        return copysign(1.0, x);

    /* For |x| <= 2^-27, tanh(x) rounds to exactly x. */
    if (s < 7.450580596923828e-9)
        return x;

    /* For |x| < 0.2554, use the polynomial tanh(x) = x/(1 + kernel(x²)).
     * This avoids catastrophic cancellation in expm1(2x) for small x,
     * which would degrade precision without FMA. */
    if (s < 0.2554128118829953416)
        return divs_(x, 1.0, kernel_(s * s));

    /* tanh(x) = expm1(2x) / (expm1(2x) + 2).
     *
     * Compute expm1(2s) = 2^q * m - 1 as a double-double t, then
     * tanh(s) = t / (t + 2) = t * recip(t + 2).
     *
     * For |x| >= 0.2554, expm1(2x) >= expm1(0.511) ≈ 0.667, so the
     * subtraction 2^q*m − 1 loses at most ~1 bit — precision is adequate.
     */
    int64_t q;
    exptab_sum_ m = sinhtab_exp_dd_(2.0 * s, &q);

    /* Scale mantissa m by 2^q (exact: multiplying by a power of 2). */
    double mhi_scaled = shift_(m.hi, q);
    double mlo_scaled = shift_(m.lo, q);

    /* t = expm1(2s) = 2^q*m - 1 as a double-double. */
    exptab_sum_ t = exptab_twosum_(mhi_scaled, -1.0);
    t.lo += mlo_scaled;

    /* tanh(s) = t / (t + 2) = t * recip(t + 2). */
    exptab_sum_ t2 = exptab_fast2sum_(t.hi + 2.0, t.lo);
    exptab_sum_ inv = sinhtab_recip_(t2);
    exptab_sum_ result = exptab_mul_(t, inv);

    return copysign(result.hi + result.lo, x);
}

#include "kernel/asin.h"
#include "kernel/atan2tint.h"
#include "kernel/exptab.h"
#include <math.h>

/* Fast-path absolute error bound for atan2.  atantab_atan_dd_ gives atan(q)
 * to ~2^-59.5 absolute error for a double-precision input q.  Using a
 * single-precision quotient q = ay/ax (rounded to nearest, error <= 0.5 ulp)
 * incurs an extra |atan'(q) * delta_q| <= 0.5 * 2^-52 absolute error since
 * atan'(q) * q <= 0.5 for all q >= 0.  The quadrant offset is a double-double
 * so that step is exact.  Total fast-path error <= 2^-59 + 2^-54 < 2^-53.
 * Using 2^-52 as ERR pushes near-midpoints to the accurate path. */
#define ATAN2_ERR 0x1p-52

/* pi/2 and pi as double-double constants. */
static const double atan2_pi_2_hi_ = 0x1.921fb54442d18p+0;  /* pi/2 */
static const double atan2_pi_2_lo_ = 0x1.1a62633145c07p-54;
static const double atan2_pi_hi_   = 0x1.921fb54442d18p+1;  /* pi */
static const double atan2_pi_lo_   = 0x1.1a62633145c07p-53;

/* Accurate path: the 192-bit Tint fallback (kernel/atan2tint.h), correctly
 * rounded down to atan2's hardest ~2^-154 ties -- beyond the 128-bit dint
 * path's ~2^-126 reach.  Preconditions: y, x both finite and nonzero. */
static double atan2_accurate_(double y, double x)
{
    return copysign(atan2_tint_mag_(fabs(x), fabs(y), x < 0.0), y);
}

double atan2(double y, double x)
{
    /* NaN propagation. */
    if (x != x || y != y)
        return x + y;

    double ay = fabs(y), ax = fabs(x);

    /* --- exact / boundary cases (C11 Annex F) --- */

    /* atan2(+-0, x >= +0) = +-0; atan2(+-0, x < 0 or -0) = +-pi. */
    if (y == 0.0) {
        if (!signbit(x))
            return y;                          /* +-0 */
        return copysign(atan2_pi_hi_, y);      /* +-pi */
    }

    /* atan2(y, +-0) = +-pi/2 for y != 0. */
    if (x == 0.0)
        return copysign(atan2_pi_2_hi_, y);

    /* atan2(+-inf, +inf) = +-pi/4; atan2(+-inf, -inf) = +-3pi/4. */
    if (ay == INFINITY) {
        if (ax == INFINITY)
            return x > 0.0 ? copysign(0x1.921fb54442d18p-1, y)   /* +-pi/4  */
                           : copysign(0x1.2d97c7f3321d2p+1, y);  /* +-3pi/4 */
        return copysign(atan2_pi_2_hi_, y);  /* +-pi/2 */
    }

    /* atan2(finite!=0, +-inf). */
    if (ax == INFINITY)
        return x > 0.0 ? copysign(0.0, y) : copysign(atan2_pi_hi_, y);

    /* --- Main computation: both y, x are finite and nonzero. --- */

    /* Reduce to q in (0, 1]:
     *   |x| >= |y|: q = |y|/|x|, angle = atan(q)           in [0, pi/4]
     *   |y| >  |x|: q = |x|/|y|, angle = pi/2 - atan(q)   in (pi/4, pi/2)
     * For the fast path, use a single-precision quotient; the dint fallback
     * handles near-midpoints with full precision via atantab_atan_dint_.
     * If the quotient underflows to 0 (|y| << |x| with extreme exponents),
     * atan(y/x) rounds to 0; handle that before the general path. */

    exptab_sum_ angle;

    if (ax >= ay) {
        double q = ay / ax;

        /* Underflow: atan(y/x) < y/x <= 2^-1075, rounds to +-0 or +-pi. */
        if (q == 0.0)
            return x > 0.0 ? copysign(0.0, y) : copysign(atan2_pi_hi_, y);

        /* Tiny q: avoid calling atantab_atan_dd_ with subnormal arg. */
        if (q < 0x1p-27)
            angle = (exptab_sum_){ q, 0.0 };
        else
            angle = atantab_atan_dd_(q);
    } else {
        double q = ax / ay;

        /* Underflow: ax/ay rounds to 0 -- result is +-pi/2. */
        if (q == 0.0)
            return copysign(atan2_pi_2_hi_, y);

        exptab_sum_ a;
        if (q < 0x1p-27)
            a = (exptab_sum_){ q, 0.0 };
        else
            a = atantab_atan_dd_(q);

        /* pi/2 - a as double-double. */
        exptab_sum_ neg_a = { -a.hi, -a.lo };
        angle = exptab_add_((exptab_sum_){ atan2_pi_2_hi_, atan2_pi_2_lo_ }, neg_a);
    }

    /* Quadrant correction for x < 0: pi - angle. */
    if (x < 0.0) {
        exptab_sum_ neg_angle = { -angle.hi, -angle.lo };
        angle = exptab_add_((exptab_sum_){ atan2_pi_hi_, atan2_pi_lo_ }, neg_angle);
    }

    double left  = angle.hi + (angle.lo - ATAN2_ERR);
    double right = angle.hi + (angle.lo + ATAN2_ERR);

    if (left != right)
        return atan2_accurate_(y, x);

    return copysign(left, y);
}

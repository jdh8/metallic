#include "kernel/asin.h"
#include "kernel/exptab.h"
#include <math.h>

/* Fast-path absolute error bound for atan2.  atantab_atan_dd_ gives atan(q)
 * to ~2^-59.5 absolute error for a double-precision input q.  Using a
 * single-precision quotient q = ay/ax (rounded to nearest, error <= 0.5 ulp)
 * incurs an extra |atan'(q) * delta_q| <= 0.5 * 2^-52 absolute error since
 * atan'(q) * q <= 0.5 for all q >= 0.  The quadrant offset is a double-double
 * so that step is exact.  Total fast-path error <= 2^-59 + 2^-54 < 2^-53.
 * Using 2^-52 as ERR pushes near-midpoints to the dint path. */
#define ATAN2_ERR 0x1p-52

/* pi/2 and pi as double-double constants. */
static const double atan2_pi_2_hi_ = 0x1.921fb54442d18p+0;  /* pi/2 */
static const double atan2_pi_2_lo_ = 0x1.1a62633145c07p-54;
static const double atan2_pi_hi_   = 0x1.921fb54442d18p+1;  /* pi */
static const double atan2_pi_lo_   = 0x1.1a62633145c07p-53;

/* pi for the dint accurate path (pi/2 no longer needed in the accurate path:
 * atan(ay/ax) is computed directly, avoiding pi/2 - atan(ax/ay)). */
static const dint_t atan2_pi_dint_ = { 0, 1,
    ((unsigned __int128)0xc90fdaa22168c234ULL << 64) | 0xc4c6628b80dc1cd1ULL };

/* Accurate path: compute atan2(y, x) using dint arithmetic.
 * Uses the atan table engine (atantab_atan_dint_xd_) with a precise quotient
 * from Dekker exact division.  For the ax<ay branch, atan(ay/ax) is computed
 * directly (avoiding explicit pi/2 subtraction) via the same engine.
 * Preconditions: y, x both finite and nonzero. */
static double atan2_accurate_(double y, double x)
{
    double ay = fabs(y), ax = fabs(x);
    dint_t r;

    /* Scale when BOTH inputs are subnormal so dint operations stay in range. */
    double ay_n = ay, ax_n = ax;
    if (ay < 0x1p-1022 && ax < 0x1p-1022) {
        ay_n *= 0x1p52;
        ax_n *= 0x1p52;
    }

    int use_dint_quot = (ay_n >= 0x1p-1022 && ax_n >= 0x1p-1022);

    /* Dekker split of ax_n requires |ax_n| < 2^996 to avoid overflow. */
    double ax_s = ax_n, ay_s = ay_n;
    if (ax_n >= 0x1p996) {
        ax_s = ax_n * 0x1p-512;
        ay_s = ay_n * 0x1p-512;
    }

    uint64_t sticky = 0;

    if (ax >= ay) {
        double q = ay / ax;
        if (use_dint_quot && q >= 0x1p-1022) {
            exptab_sum_ prod = exptab_prod_(q, ax_s);
            double q_lo = (ay_s - prod.hi - prod.lo) / ax_s;
            dint_t qd = atantab_dint_from_dd_(q, q_lo);
            r = atantab_atan_dint_xd_ex_(&qd, q, &sticky);
        } else {
            dint_t qd = dint_from_f64_(q);
            r = atantab_atan_dint_xd_ex_(&qd, q, &sticky);
        }
    } else {
        /* Compute atan(ay/ax) directly (angle in (pi/4, pi/2]). */
        double q = ay / ax;
        if (use_dint_quot && q < 0x1p1023) {
            exptab_sum_ prod = exptab_prod_(q, ax_s);
            double q_lo = (ay_s - prod.hi - prod.lo) / ax_s;
            dint_t qd = atantab_dint_from_dd_(q, q_lo);
            r = atantab_atan_dint_xd_ex_(&qd, q, &sticky);
        } else {
            dint_t qd = dint_from_f64_(q);
            r = atantab_atan_dint_xd_ex_(&qd, q, &sticky);
        }
    }

    /* Quadrant correction for x < 0: pi - r. */
    if (x < 0.0) {
        r.sgn = !r.sgn;
        r = dint_add_(&atan2_pi_dint_, &r);
        sticky = 0;
    }

    double result;
    if (r.ex >= -1022) {
        result = dint_to_f64_ex_(&r, sticky);
    } else {
        dint_t scaled = r;
        scaled.ex += 64;
        result = scalbn(dint_to_f64_ex_(&scaled, sticky), -64);
    }
    return copysign(result, y);
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

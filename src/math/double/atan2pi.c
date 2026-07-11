/* C23 atan2pi: atan2(y, x)/pi, correctly rounded.
 *
 * Port of metallic-rs `atan2pi` (src/f64_/atan.rs ~ line 2601), re-tailored
 * to lift this repo's atan2 engines by 1/pi (kernel/invpi.h).  Every special
 * ray is exactly representable in half-turns: (+-0, x>0) = +-0, (+-0, x<0) =
 * +-1, (y, +-0) = +-1/2, (+-inf, +inf) = +-1/4, (+-inf, -inf) = +-3/4,
 * (+-inf, finite) = +-1/2, (finite, -+inf) = +-1/+-0, and the finite
 * diagonal |y| = |x| lands on +-1/4 / +-3/4 through the ordinary lift.
 *
 * The fast leg upgrades atan2.c's plain quotient to an exact double-double
 * one (Dekker residual on legs scaled so the larger lands in [1, 2)), folded
 * into the atantab engine through the derivative 1/(1+q^2) -- after the 1/pi
 * lift a plain quotient's half-ulp rounding would exceed any sound gate
 * (metallic-rs atan2pi_mag ~ 2554 makes the same upgrade for its tiny band).
 * The deep tier is atan2.c's 192-bit Tint ladder with one more Tint product
 * by 1/pi before the rounding, the ulp bounds doubled to absorb it
 * (metallic-rs atan2pi_tint_mag ~ 2485); it also owns the deep tail
 * q <= 2^-955 of the direct quadrant, whose results reach the subnormals. */

#include "kernel/atantab.h"
#include "kernel/atan2tint.h"
#include "kernel/invpi.h"
#include <math.h>

/* Fast-leg absolute error bound: atan2.c's engine error is dominated by
 * atantab_atan_dd_'s certified 2^-58 (atan.c's ATAN_ERR) once the quotient
 * is exact double-double; scaled by 1/pi, with the quadrant folds (~2^-103)
 * inside the margin.  The lift product's relative |w|*2^-102 is charged
 * separately at the gate. */
#define ATAN2PI_ERR 0x1.45f306dc9c883p-60

/* pi/2 and pi as double-double constants (as in atan2.c). */
static const double atan2pi_pi_2_hi_ = 0x1.921fb54442d18p+0;
static const double atan2pi_pi_2_lo_ = 0x1.1a62633145c07p-54;
static const double atan2pi_pi_hi_   = 0x1.921fb54442d18p+1;
static const double atan2pi_pi_lo_   = 0x1.1a62633145c07p-53;

/* atan2_tint_mag_ (kernel/atan2tint.h) lifted by 1/pi: the same 192-bit
 * ladder with one more tint product before the rounding, the ulp bounds
 * doubled (532/1048/1324 for 266/524/662), and the tiny-ratio shortcut's
 * bound widened to 8 (metallic-rs atan2pi_tint_mag).  The tiny-ratio and
 * deep-tail results may be subnormal; tint_to_f64_ rounds those correctly. */
static double atan2pi_tint_mag_(double a, double b, _Bool x_negative)
{
    _Bool inv = b > a;
    double num = inv ? a : b;
    double den = inv ? b : a;
    tint_t z = tint_div_d_(num, den);

    /* atan(z)/pi ~ z/pi with CORE-MATH's hair-toward-zero perturbation;
     * only sound in the direct quadrant (see atan2_tint_mag_). */
    if (!inv && !x_negative && z.ex <= -96) {
        uint64_t borrow = z.lo < 2;
        z.lo -= 2;
        z.hi -= (unsigned __int128)borrow;
        z = tint_mul_(&z, &invpi_tint_);
        return tint_to_f64_(&z, 8);
    }

    tint_t p = atan2_p_tint_[29];
    tint_t q = atan2_q_tint_[29];

    for (int i = 28; i >= 0; --i) {
        p = tint_mul_(&p, &z);
        q = tint_mul_(&q, &z);
        p = tint_add_(&p, &atan2_p_tint_[i]);
        q = tint_add_(&q, &atan2_q_tint_[i]);
    }
    p = tint_mul_(&p, &z);
    z = tint_div_(&p, &q);

    if (inv) {
        tint_t nz = tint_neg_(z);
        z = tint_add_(&tint_pi2_, &nz);
    }
    if (x_negative) {
        tint_t nz = tint_neg_(z);
        z = tint_add_(&tint_pi_, &nz);
    }

    uint64_t err = x_negative ? 532 : inv ? 1048 : 1324;
    z = tint_mul_(&z, &invpi_tint_);
    return tint_to_f64_(&z, err);
}

double atan2pi(double y, double x)
{
    if (x != x || y != y)
        return x + y;

    double ay = fabs(y), ax = fabs(x);

    /* --- exact rays (C23 F.10.1.4, all exact in half-turns) --- */

    if (y == 0.0) {
        if (!signbit(x))
            return y;                    /* +-0 */
        return copysign(1.0, y);         /* +-1 */
    }

    if (x == 0.0)
        return copysign(0.5, y);

    if (ay == INFINITY) {
        if (ax == INFINITY)
            return x > 0.0 ? copysign(0.25, y) : copysign(0.75, y);
        return copysign(0.5, y);
    }

    if (ax == INFINITY)
        return x > 0.0 ? copysign(0.0, y) : copysign(1.0, y);

    /* --- main: both finite and nonzero; reduce to q = small/big in (0, 1] --- */

    int swapped = ay > ax;
    double big = swapped ? ay : ax;
    double small = swapped ? ax : ay;
    double q = small / big;

    /* Fully underflowed quotient: the true ratio is <= 2^-1075, so the
     * angle rounds to the axis it hugs. */
    if (q == 0.0) {
        if (swapped)
            return copysign(0.5, y);
        return x > 0.0 ? copysign(0.0, y) : copysign(1.0, y);
    }

    /* Deep tail of the direct quadrant: below q ~ 2^-955 the relative gate
     * width itself goes subnormal (and the results eventually do), so the
     * 192-bit tier owns the rounding outright (metallic-rs atan2pi_mag). */
    if (!swapped && x > 0.0 && q <= 0x1p-955)
        return copysign(atan2pi_tint_mag_(ax, ay, 0), y);

    /* Exact two-power rescale putting big in [1, 2) (ratio preserved): keeps
     * the Dekker split of the residual away from overflow and its products
     * out of the subnormals.  Legs below 2^-900 are first lifted by 2^200
     * (exact) so the exponent extraction sees a normal value. */
    double bs = big, ss = small;
    if (bs < 0x1p-900) {
        bs *= 0x1p200;
        ss *= 0x1p200;
    }
    uint64_t bb = reinterpret(uint64_t, bs);
    int64_t eb = (int64_t)(bb >> 52);
    double bn = reinterpret(double, (bb & (~0ULL >> 12)) | (1023ULL << 52));
    uint64_t scb = eb < 2046 ? (uint64_t)(2046 - eb) << 52 : 1ULL << 51;
    double sn = ss * reinterpret(double, scb);

    int tiny_direct = 0;
    exptab_sum_ a;

    if (q < 0x1p-27) {
        /* Tiny ratio: after the 1/pi lift the plain quotient's half-ulp
         * rounding breaks correct rounding, so take the exact double-double
         * quotient and fold the -q^3/3 term; the dropped q^5 term and the
         * residual roundings stay below the 2^-100 relative gate of the
         * direct quadrant (elsewhere the angle is O(1) and the absolute
         * gate dwarfs them). */
        double qh = sn / bn;
        exptab_sum_ e = exptab_prod_(qh, bn);
        double ql = ((sn - e.hi) - e.lo) / bn;
        a = (exptab_sum_){ qh, ql + qh * (qh * qh) * (-1.0 / 3) };
        tiny_direct = !swapped && x > 0.0;
    } else {
        exptab_sum_ e = exptab_prod_(q, bn);
        double ql = ((sn - e.hi) - e.lo) / bn;
        a = atantab_atan_dd_(q);
        a.lo += ql / (1.0 + q * q); /* quotient low word through atan' */
    }

    /* Quadrant folds, as in atan2.c: pi/2 - a when the legs swapped,
     * pi - angle when x < 0. */
    exptab_sum_ angle = a;

    if (swapped) {
        exptab_sum_ neg_a = { -a.hi, -a.lo };
        angle = exptab_add_((exptab_sum_){ atan2pi_pi_2_hi_, atan2pi_pi_2_lo_ }, neg_a);
    }
    if (x < 0.0) {
        exptab_sum_ neg = { -angle.hi, -angle.lo };
        angle = exptab_add_((exptab_sum_){ atan2pi_pi_hi_, atan2pi_pi_lo_ }, neg);
    }

    exptab_sum_ w = exptab_mul_(angle, (exptab_sum_){ invpi_hi_, invpi_lo_ });
    double eps = tiny_direct ? w.hi * 0x1p-100
                             : ATAN2PI_ERR + fabs(w.hi) * 0x1p-102;
    double left = w.hi + (w.lo - eps);
    double right = w.hi + (w.lo + eps);

    if (left != right)
        return copysign(atan2pi_tint_mag_(ax, ay, x < 0.0), y);

    return copysign(left, y);
}

#include "kernel/lncells.h"
#include "kernel/log1ptab.h"
#include "kernel/logtab.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Ziv gate for the lean table leg (metallic-rs LN_ZIV_EPS): the exact-z leg
 * plus the c fold is good to < 2^-66 absolute, so 2^-63 keeps an 8x margin
 * (certified by metallic-rs ziv_soundness::log1p_table_leg_is_sound).  The
 * gate is absolute, so straddles get likelier as the result shrinks toward
 * |x| ~ 2^-8; the double-double tier below keeps those correct. */
#define LN_ZIV_EPS 0x1p-63

/* Ziv error bounds for the faithful double-double middle tier.  In the
 * small-|x| branch the result tracks x, so the error is relative (a few ulp of
 * the result); in the table branch it is essentially a fixed double-double
 * bound.  Where the test fails the result is too close to a rounding boundary
 * to round from the fast path, and the triple-double refine (~150-bit)
 * decides it. */
#define LOG1P_REL 0x1p-60
#define LOG1P_ERR 0x1p-67

double log1p(double x)
{
    if (x != x || x == 0)
        return x;   /* NaN → NaN, ±0 → ±0 */
    if (x < -1)
        return NAN;
    if (x == -1)
        return -INFINITY;
    if (x == INFINITY)
        return x;

    /* log1p(x) - x = -x²/2·(1 - 2x/3 + …): the true value sits strictly on
     * the toward-zero side of x for x > 0 and the away-from-zero side for
     * x < 0, at distance in (0, x²/2].  With |x| < 2^-53 (binade e ≤ -54)
     * every half-gap to a neighboring midpoint is at least 2^(e-54) -- the
     * binding corner is x = 2^e exactly, whose downward gap shrinks -- and
     * x²/2 = 2^(2e-1) ≤ 2^(e-55) < 2^(e-54) for e ≤ -54.  No tie arises, so
     * log1p(x) rounds to x. */
    if (fabs(x) < 0x1p-53)
        return x;

    /* Small |x|: evaluate ln(1+x) = x·P(x) directly, avoiding table cancellation. */
    if (fabs(x) < 1.0 / 256.0) {
        exptab_sum_ r = logtab_ln1p_kernel_((exptab_sum_){ x, 0.0 });
        double err = LOG1P_REL * fabs(r.hi);
        double left = r.hi + (r.lo - err);
        double right = r.hi + (r.lo + err);
        return left == right ? left : log1ptab_refine_(x, r.hi + r.lo);
    }

    /* Carry 1 + x exactly as s + c via Fast2Sum, preserving lost bits of x. */
    double s, c;
    if (fabs(x) <= 1.0) {
        s = 1.0 + x;
        c = x - (s - 1.0);
    } else {
        s = x + 1.0;
        c = 1.0 - (s - x);
    }

    /* The lean exact-z leg on s with the tail c folded into the low word
     * (see lncells_ln1p_raw_). */
    exptab_sum_ f = lncells_ln1p_raw_(s, c);
    double left = f.hi + (f.lo - LN_ZIV_EPS);
    double right = f.hi + (f.lo + LN_ZIV_EPS);
    if (left == right)
        return left;

    /* Middle tier: reduce s = 2^e * m, compute r = m * INV[i] − 1 ∈ [−1/256, 1/256]. */
    int64_t e;
    int i;
    exptab_sum_ r;
    logtab_reduce_(s, &e, &i, &r);

    /* Fold the tail c into r: r_full = r + c * 2^-e * INV[i].
     * c * 2^-e is the tail of 1+x = s+c scaled to the mantissa binade;
     * multiplying by INV[i] gives the delta in the reduced-argument space.
     * Use Dekker product so the double-double addition is accurate. */
    if (e > -1000) {
        double scaled_c = c * shift_(1.0, -e);
        exptab_sum_ dd = exptab_prod_(scaled_c, logtab_inv_[i]);
        r = exptab_add_(r, dd);
    }

    double ef = (double)e;
    exptab_sum_ e_ln2 = { ef * logtab_ln2_hi_, ef * logtab_ln2_lo_ };
    exptab_sum_ L = { logtab_l_[i][0], logtab_l_[i][1] };
    exptab_sum_ result = exptab_add_(exptab_add_(e_ln2, L), logtab_ln1p_kernel_(r));
    left = result.hi + (result.lo - LOG1P_ERR);
    right = result.hi + (result.lo + LOG1P_ERR);
    return left == right ? left : log1ptab_refine_(x, result.hi + result.lo);
}

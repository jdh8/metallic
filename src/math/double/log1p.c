#include "kernel/log1ptab.h"
#include "kernel/logtab.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Ziv error bounds for the faithful double-double fast path.  In the small-|x|
 * branch the result tracks x, so the error is relative (a few ulp of the
 * result); in the table branch it is essentially a fixed double-double bound.
 * Where the test fails the result is too close to a rounding boundary to round
 * from the fast path, and the triple-double refine (~150-bit) decides it. */
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

    /* Reduce s = 2^e * m, compute r = m * INV[i] − 1 ∈ [−1/256, 1/256]. */
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
        exptab_sum_ delta = exptab_prod_(scaled_c, logtab_inv_[i]);
        r = exptab_add_(r, delta);
    }

    double ef = (double)e;
    exptab_sum_ e_ln2 = { ef * logtab_ln2_hi_, ef * logtab_ln2_lo_ };
    exptab_sum_ L = { logtab_l_[i][0], logtab_l_[i][1] };
    exptab_sum_ result = exptab_add_(exptab_add_(e_ln2, L), logtab_ln1p_kernel_(r));
    double left = result.hi + (result.lo - LOG1P_ERR);
    double right = result.hi + (result.lo + LOG1P_ERR);
    return left == right ? left : log1ptab_refine_(x, result.hi + result.lo);
}

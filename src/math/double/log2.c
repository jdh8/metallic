#include "kernel/dint.h"
#include "kernel/lncells.h"
#include "kernel/logtab.h"
#include <math.h>

/* Ziv gate for the lean leg after the base conversion: the base-e leg's
 * < 2^-66 absolute error (see lncells_assemble_) scaled by log2(e), plus
 * <~ 2^-67 the double-double multiply adds on its own -- the exact hi product
 * (Dekker here, fma in metallic-rs) drops a.lo*b.lo <= 2^-71 and rounds two
 * low-word sums at <= 2^-68 each.  2^-63 * log2(e), rounded up, keeps the
 * base-e leg's 8x margin (metallic-rs LOG2_ZIV_EPS).  Exact cases stay exact:
 * for x = 2^k the leg lands within the gate of the representable k, so both
 * gate ends round to k. */
#define LOG2_ZIV_EPS 1.5641377104174595e-19

/* Middle-tier max absolute error: the natural-log double-double error scaled
 * by log2(e) ~ 1.44 plus the extra double-double multiply, so use an ERR a few
 * bits larger than log's 2^-78. */
#define LOG2_ERR 0x1p-75

static double log2_accurate_(double x)
{
    dint_t ln = dint_ln_(dint_from_f64_(x));
    dint_t r = dint_mul_(&ln, &dint_log2e_);
    return dint_to_f64_(&r);
}

double log2(double x)
{
    if (x != x)
        return x;
    if (x < 0)
        return NAN;
    if (x == 0)
        return -INFINITY;
    if (x == 1 || x == INFINITY)
        return x - 1;

    exptab_sum_ log2e = { logtab_log2e_hi_, logtab_log2e_lo_ };

    exptab_sum_ f = exptab_mul_(lncells_ln_(x), log2e);
    double left = f.hi + (f.lo - LOG2_ZIV_EPS);
    double right = f.hi + (f.lo + LOG2_ZIV_EPS);
    if (left == right)
        return left;

    exptab_sum_ r = exptab_mul_(logtab_ln_dd_(x), log2e);
    left = r.hi + (r.lo - LOG2_ERR);
    right = r.hi + (r.lo + LOG2_ERR);
    return left == right ? left : log2_accurate_(x);
}

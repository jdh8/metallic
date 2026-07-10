#include "kernel/dint.h"
#include "kernel/lncells.h"
#include "kernel/logtab.h"
#include <math.h>

/* Ziv gate for the lean leg after the base conversion (metallic-rs
 * LOG10_ZIV_EPS): the base-e leg's < 2^-66 absolute error (see
 * lncells_assemble_) shrinks through the * log10(e) ~ 0.434 scaling, and the
 * double-double multiply adds <~ 2^-67 of low-word rounding on its own (exact
 * Dekker hi product in place of metallic-rs's fma, dropped a.lo*b.lo, two
 * low-word sums) -- a flat 2^-63 >= 2^-63 * log10(e) keeps more than the
 * base-e leg's 8x margin. */
#define LOG10_ZIV_EPS 0x1p-63

/* Middle-tier max absolute error: like log2, an extra double-double multiply
 * on top of the natural-log fast path, so use an ERR a few bits larger than
 * log's 2^-78. */
#define LOG10_ERR 0x1p-75

static double log10_accurate_(double x)
{
    dint_t ln = dint_ln_(dint_from_f64_(x));
    dint_t r = dint_mul_(&ln, &dint_log10e_);
    return dint_to_f64_(&r);
}

double log10(double x)
{
    if (x != x)
        return x;
    if (x < 0)
        return NAN;
    if (x == 0)
        return -INFINITY;
    if (x == 1 || x == INFINITY)
        return x - 1;

    exptab_sum_ log10e = { logtab_log10e_hi_, logtab_log10e_lo_ };

    exptab_sum_ f = exptab_mul_(lncells_ln_(x), log10e);
    double left = f.hi + (f.lo - LOG10_ZIV_EPS);
    double right = f.hi + (f.lo + LOG10_ZIV_EPS);
    if (left == right)
        return left;

    exptab_sum_ r = exptab_mul_(logtab_ln_dd_(x), log10e);
    left = r.hi + (r.lo - LOG10_ERR);
    right = r.hi + (r.lo + LOG10_ERR);
    return left == right ? left : log10_accurate_(x);
}

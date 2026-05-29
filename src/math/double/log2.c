#include "kernel/dint.h"
#include "kernel/logtab.h"
#include <math.h>

/* log2 = ln * log2(e): the fast path is the natural-log error scaled by
 * log2(e) ~ 1.44 plus the extra double-double multiply, so use an ERR a few
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

    exptab_sum_ ln = logtab_ln_dd_(x);
    exptab_sum_ log2e = { logtab_log2e_hi_, logtab_log2e_lo_ };
    exptab_sum_ r = exptab_mul_(ln, log2e);
    double left = r.hi + (r.lo - LOG2_ERR);
    double right = r.hi + (r.lo + LOG2_ERR);
    return left == right ? left : log2_accurate_(x);
}

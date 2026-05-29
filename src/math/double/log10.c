#include "kernel/dint.h"
#include "kernel/logtab.h"
#include <math.h>

/* log10 = ln * log10(e): like log2, an extra multiply on top of the natural-log
 * fast path, so use an ERR a few bits larger than log's 2^-78. */
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

    exptab_sum_ ln = logtab_ln_dd_(x);
    exptab_sum_ log10e = { logtab_log10e_hi_, logtab_log10e_lo_ };
    exptab_sum_ r = exptab_mul_(ln, log10e);
    double left = r.hi + (r.lo - LOG10_ERR);
    double right = r.hi + (r.lo + LOG10_ERR);
    return left == right ? left : log10_accurate_(x);
}

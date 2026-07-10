#include "kernel/dint.h"
#include "kernel/lncells.h"
#include "kernel/logtab.h"
#include <math.h>

/* Ziv gate for the lean fast leg, as an absolute error bound: the exact-z leg
 * is good to < 2^-66 absolute (see lncells_assemble_), so 2^-63 keeps an 8x
 * margin.  Being absolute, the gate only forces a fallback for |ln x| <~ 2^-10,
 * i.e. x within ~2^-10 of 1.  Ported from metallic-rs LN_ZIV_EPS. */
#define LN_ZIV_EPS 0x1p-63

/* Middle-tier max absolute error: the double-double kernel is faithful (0
 * errors over 120M random inputs), so its error is well under 1 ulp of the
 * result; 2^-78 comfortably covers it while keeping the Ziv fallback rare. */
#define LOG_ERR 0x1p-78

static double log_accurate_(double x)
{
    dint_t r = dint_ln_(dint_from_f64_(x));
    return dint_to_f64_(&r);
}

double log(double x)
{
    if (x != x)
        return x;
    if (x < 0)
        return NAN;
    if (x == 0)
        return -INFINITY;
    if (x == 1 || x == INFINITY)
        return x - 1;

    exptab_sum_ f = lncells_ln_(x);
    double left = f.hi + (f.lo - LN_ZIV_EPS);
    double right = f.hi + (f.lo + LN_ZIV_EPS);
    if (left == right)
        return left;

    exptab_sum_ r = logtab_ln_dd_(x);
    left = r.hi + (r.lo - LOG_ERR);
    right = r.hi + (r.lo + LOG_ERR);
    return left == right ? left : log_accurate_(x);
}

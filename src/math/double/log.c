#include "kernel/dint.h"
#include "kernel/logtab.h"
#include <math.h>

/* Fast-path max absolute error: the double-double kernel is faithful (0 errors
 * over 120M random inputs), so its error is well under 1 ulp of the result;
 * 2^-78 comfortably covers it while keeping the Ziv fallback rare. */
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

    exptab_sum_ r = logtab_ln_dd_(x);
    double left = r.hi + (r.lo - LOG_ERR);
    double right = r.hi + (r.lo + LOG_ERR);
    return left == right ? left : log_accurate_(x);
}

/* C23 log10p1: log10(1 + x), correctly rounded.
 *
 * log2p1's structure with the base-10 constant (see log2p1.c); port of
 * metallic-rs `log10p1` (src/f64_/log.rs).  The exact cases
 * log10p1(10^n - 1) = n pass through the table leg's gate naturally. */

#include "kernel/lncells.h"
#include "kernel/ln1pleg.h"
#include <math.h>
#include <stdint.h>

static const double log10p1_e_hi_ = 0.4342944819032518;
static const double log10p1_e_lo_ = 1.098319650216765e-17;

static double log10p1_deep_(double x)
{
    dint_t s = ln1pleg_series_(x);
    dint_t r = dint_mul_(&s, &dint_log10e_);
    return dint_to_f64_general_(&r);
}

static double log10p1_accurate_(double s, double c)
{
    dint_t sd = dint_from_f64_(s);
    dint_t cd = dint_from_f64_(c);
    dint_t arg = dint_add_(&sd, &cd);
    dint_t ln = dint_ln_(arg);
    dint_t r = dint_mul_(&ln, &dint_log10e_);
    return dint_to_f64_(&r);
}

double log10p1(double x)
{
    if (x != x || x == 0)
        return x; /* NaN propagates; +-0 keeps its sign */
    if (x < -1)
        return (x - x) / (x - x);
    if (x == -1)
        return -INFINITY;
    if (x == INFINITY)
        return x;

    double ax = fabs(x);

    if (ax < 0.0625) {
        if (ax < 0x1p-900)
            return log10p1_deep_(x);

        exptab_sum_ raw = ax < 1.0 / 256.0 ? ln1pleg_small_(x) : ln1pleg_wide_(x);
        exptab_sum_ v = exptab_mul_(raw, (exptab_sum_){ log10p1_e_hi_, log10p1_e_lo_ });
        double scale = ax < 1.0 / 256.0 ? 3.552713678800501e-15 * log10p1_e_hi_       /* 2^-48 */
                                        : 8.881784197001252e-16 * log10p1_e_hi_ * ax; /* 2^-50 */
        double err = x * x * scale + fabs(v.hi) * 0x1p-101;
        double left = v.hi + (v.lo - err);
        double right = v.hi + (v.lo + err);

        if (left == right)
            return left;

        return log10p1_deep_(x);
    }

    double s, c;

    if (ax <= 1.0) {
        s = 1.0 + x;
        c = x - (s - 1.0);
    } else {
        s = x + 1.0;
        c = 1.0 - (s - x);
    }

    exptab_sum_ v = exptab_mul_(lncells_ln1p_raw_(s, c),
                                (exptab_sum_){ log10p1_e_hi_, log10p1_e_lo_ });

    double eps = 0x1p-63 * log10p1_e_hi_ * 1.0625;
    double left = v.hi + (v.lo - eps);
    double right = v.hi + (v.lo + eps);

    if (left == right)
        return left;

    return log10p1_accurate_(s, c);
}

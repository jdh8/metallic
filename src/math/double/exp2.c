#include "kernel/exptab.h"
#include "kernel/exp2tab.h"
#include <math.h>
#include <stdint.h>

double exp2(double x)
{
    if (isnan(x))
        return x;

    if (x >= 1024)
        return INFINITY;

    if (x <= -1075)
        return 0;

    /* n = round(N * x), j = n mod N, q = n / N.
     * sigma = x * N - n is exact (N is a power of 2, Sterbenz). */
    double scaled = rint(x * (double)EXPTAB_N);
    int64_t n = (int64_t)scaled;
    int j = (int)(n & (EXPTAB_N - 1));
    int64_t q = n >> 7;

    /* r = sigma * (ln2/N) as a double-double: the argument to exp(r). */
    double sigma = x * (double)EXPTAB_N - scaled;
    exptab_sum_ product = exptab_prod_(sigma, exptab_ln2_over_n_hi_);
    exptab_sum_ r = exptab_fast2sum_(product.hi,
        product.lo + sigma * exptab_ln2_over_n_lo_);

    exptab_sum_ p = exptab_mantissa_(j, &q, r);

    if (q >= -1022) {
        double s = p.hi + p.lo;
        double e = (p.hi - s) + p.lo;

        if (fabs(e) + 0x1p-98 < 0x1p-53)
            return shift_(s, q);

        /* Near a rounding midpoint: look up the precomputed table. */
        double wc = exp2_wc_lookup_(x);
        return wc ? wc : shift_(s, q);
    }

    /* Subnormal path. */
    {
        double wc = exp2_wc_lookup_(x);
        if (wc) return wc;
    }

    int64_t shift = q + 1074;
    double high = shift_(p.hi, shift);
    double low = p.lo ? shift_(p.lo, shift) : 0.0;
    double m0 = rint(high);
    double m = m0 + rint((high - m0) + low);
    return m * 0x1p-1074;
}

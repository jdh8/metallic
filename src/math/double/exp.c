#include "kernel/exptab.h"
#include <math.h>
#include <stdint.h>

double exp(double x)
{
    if (isnan(x))
        return x;

    /* Largest x with exp(x) finite is 0x1.62e42fefa39efp+9; above it the
     * reconstruction would overflow its exponent rather than yield infinity. */
    if (x > 0x1.62e42fefa39efp+9)
        return INFINITY;

    if (x <= -745.133219101941)
        return 0;

    /* n = round(128 * x / ln2) = 128*q + j, reducing x to r = x - n*ln2/128
     * with |r| <= ln2/256.  scaled * hi is exact (hi has 17 trailing zero bits,
     * |scaled| < 2^18); the lo/lo2 tail is folded in as a double-double so r is
     * accurate to ~2^-100. */
    double scaled = rint(x * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;
    int j = n & (EXPTAB_N - 1);
    int64_t q = n >> 7;

    double a = x - scaled * exptab_ln2_over_n_hi_;
    exptab_sum_ blo = exptab_prod_(scaled, exptab_ln2_over_n_lo_);
    exptab_sum_ r = exptab_twosum_(a, -blo.hi);
    r = exptab_fast2sum_(r.hi, r.lo - blo.lo - scaled * exptab_ln2_over_n_lo2_);

    return exptab_reconstruct_(j, q, r);
}

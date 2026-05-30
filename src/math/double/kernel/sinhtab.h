#ifndef METALLIC_KERNEL_SINHTAB_H
#define METALLIC_KERNEL_SINHTAB_H

/* Shared double-double helpers for sinh, cosh, tanh — ported from
 * metallic-rs f64.rs (cosh_sinh_mantissa / exp_dd / Sum::recip).
 * All use exptab.h infrastructure; no scalar FMA (WASM constraint). */

#include "exptab.h"

/* Argument reduction for exp_dd: x = q*ln2 + j*(ln2/128) + r, |r|<=ln2/256. */
static inline exptab_sum_ sinhtab_reduce_(double x, int *j_out, int64_t *q_out)
{
    double scaled = rint(x * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;
    *j_out = (int)(n & 127);
    *q_out = n >> 7;

    double a = x - scaled * exptab_ln2_over_n_hi_;
    exptab_sum_ blo = exptab_prod_(scaled, exptab_ln2_over_n_lo_);
    exptab_sum_ r = exptab_twosum_(a, -blo.hi);
    r = exptab_fast2sum_(r.hi, r.lo - blo.lo - scaled * exptab_ln2_over_n_lo2_);
    return r;
}

/* exp(x) as double-double (m, *q): exp(x) = 2^(*q) * m, m in [1,2). */
static inline exptab_sum_ sinhtab_exp_dd_(double x, int64_t *q_out)
{
    int j;
    int64_t q = 0;
    exptab_sum_ r = sinhtab_reduce_(x, &j, &q);
    exptab_sum_ m = exptab_mantissa_(j, &q, r);
    *q_out = q;
    return m;
}

/* FMA-free double-double reciprocal 1/m.
 * Newton: hi = 1/m.hi; error = 1 - hi*m via Dekker; lo corrects it. */
static inline exptab_sum_ sinhtab_recip_(exptab_sum_ m)
{
    double hi = 1.0 / m.hi;
    exptab_sum_ e = exptab_prod_(hi, m.hi);
    double lo = (1.0 - e.hi - e.lo - hi * m.lo) / m.hi;
    return (exptab_sum_){ hi, lo };
}

/* cosh/sinh mantissa: given exp(x) = 2^q * m, form m ± 2^(-2q)/m.
 * add != 0 => cosh (plus), add == 0 => sinh (minus).
 * Returns the double-double mantissa; caller scales by 2^(q-1). */
static inline exptab_sum_ sinhtab_mantissa_(double x, int add, int64_t *q_out)
{
    int64_t q;
    exptab_sum_ m = sinhtab_exp_dd_(x, &q);

    /* t = 2^(-2q)/m ≈ exp(-x); negligible when 2q > 53 (< 0.5 ulp of result). */
    double scale = q > 26 ? 0.0 : shift_(1.0, -2 * q);
    exptab_sum_ rec = sinhtab_recip_(m);
    exptab_sum_ t = { rec.hi * scale, rec.lo * scale };

    /* Use TwoSum for the high parts to capture rounding; then fold in the lo words.
     * Fast2Sum is invalid when m.hi and t.hi are close (sinh small-x cancellation). */
    exptab_sum_ s;
    if (add)
        s = exptab_twosum_(m.hi, t.hi);
    else
        s = exptab_twosum_(m.hi, -t.hi);
    exptab_sum_ mantissa = exptab_fast2sum_(s.hi, s.lo + (add ? m.lo + t.lo : m.lo - t.lo));

    *q_out = q;
    return mantissa;
}

#endif /* METALLIC_KERNEL_SINHTAB_H */

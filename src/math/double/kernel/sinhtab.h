#ifndef METALLIC_KERNEL_SINHTAB_H
#define METALLIC_KERNEL_SINHTAB_H

/* Shared double-double machinery for sinh, cosh, tanh — ported from
 * metallic-rs f64 hyp.rs.  All built on the exptab.h double-double exp kernel;
 * no scalar FMA (WASM constraint).
 *
 * The exp mantissa is the same ~2^-100 double-double exp.c uses, so each
 * hyperbolic function reaches correct rounding the same way exp does: a Ziv
 * gate on the combined mantissa's rounding residual certifies the fast result,
 * and the rare near-midpoint straddles defer to a CORE-MATH worst-case table
 * (see sinh_wc_/cosh_wc_/tanh_wc_ in the .c files). */

#include "exptab.h"
#include "../../reinterpret.h"

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

/* Normalize a positive double-double `v` (value = 2^base_exp * v) so its high
 * word lies in [1,2); fold the binary exponent into *e_out. */
static inline exptab_sum_ sinhtab_normalize_(exptab_sum_ v, int64_t base_exp, int64_t *e_out)
{
    int64_t k = (int64_t)((reinterpret(uint64_t, v.hi) >> 52) & 0x7ff) - 1023;
    *e_out = base_exp + k;
    /* shift_ is bit-manipulated scalbn; guard the exact-zero low word, which it
     * would otherwise turn into a spurious denormal. */
    return (exptab_sum_){ shift_(v.hi, -k), v.lo == 0.0 ? 0.0 : shift_(v.lo, -k) };
}

/* cosh/sinh mantissa: exp(x) = 2^q * m, form m ± 2^(-2q)/m so that
 * ½(e^x ± e^-x) = 2^(q-1) * (m ± 2^(-2q)/m).  add != 0 => cosh, == 0 => sinh.
 * Returns the result normalized to a [1,2) mantissa with the binary exponent in
 * *e_out (so the value is 2^(*e_out) * mantissa). */
static inline exptab_sum_ sinhtab_combine_(double x, int add, int64_t *e_out)
{
    int64_t q;
    exptab_sum_ m = sinhtab_exp_dd_(x, &q);

    /* t = 2^(-2q)/m ≈ e^-x relative to e^x.  Drop it only once it falls below
     * the ~2^-100 path floor (2q > 105); the old `q > 26` cutoff dropped a term
     * still worth ~0.25 ulp near x ≈ 18.7, mis-rounding the large-|x| band. */
    exptab_sum_ combined;
    if (q >= 53) {
        combined = m;
    } else {
        double scale = shift_(1.0, -2 * q);
        exptab_sum_ rec = sinhtab_recip_(m);
        exptab_sum_ t = { rec.hi * scale, rec.lo * scale };

        /* TwoSum the high parts (Fast2Sum is invalid when m.hi ≈ t.hi for the
         * sinh near-cancellation), then fold in the low words. */
        exptab_sum_ s = add ? exptab_twosum_(m.hi, t.hi) : exptab_twosum_(m.hi, -t.hi);
        combined = exptab_fast2sum_(s.hi, s.lo + (add ? m.lo + t.lo : m.lo - t.lo));
    }

    return sinhtab_normalize_(combined, q - 1, e_out);
}

/* tanh mantissa: tanh(x) = E/(E+2) with E = e^(2x) - 1 = 2^q*m - 1.  Returns
 * the result normalized to a [1,2) mantissa with the binary exponent in *e_out.
 * Valid for x >= 1/8 (E >= e^0.25 - 1 ≈ 0.28, so E/(E+2) never cancels). */
static inline exptab_sum_ sinhtab_tanh_combine_(double x, int64_t *e_out)
{
    int64_t q;
    exptab_sum_ m = sinhtab_exp_dd_(2.0 * x, &q);
    double mhi = shift_(m.hi, q), mlo = shift_(m.lo, q);   /* e^(2x) as a pair */

    exptab_sum_ e = exptab_twosum_(mhi, -1.0);
    e.lo += mlo;
    /* E + 2 must keep the carry of E.hi + 2 — a Fast2Sum on (E.hi + 2) would
     * round it away, capping the path at ~2^-53 (the old tanh.c bug). */
    exptab_sum_ e2 = exptab_twosum_(e.hi, 2.0);
    e2.lo += e.lo;

    exptab_sum_ inv = sinhtab_recip_(e2);
    return sinhtab_normalize_(exptab_mul_(e, inv), 0, e_out);
}

/* Ziv-gated round of the normalized mantissa (value = 2^e * mant).  Sets *out
 * to the fast result `shift_(round(mant), e)` and returns nonzero when the
 * ~2^-100 path is provably far enough from a rounding midpoint to certify it.
 * On a straddle (return 0) the caller consults the worst-case table. */
static inline int sinhtab_round_(exptab_sum_ mant, int64_t e, double *out)
{
    double r = mant.hi + mant.lo;
    double resid = (mant.hi - r) + mant.lo;
    *out = shift_(r, e);
    /* A midpoint of a [1,2) mantissa sits 2^-53 away; the path errs by <2^-97,
     * so >2^-97 from a midpoint the rounding is settled (cf. exp.c). */
    return fabs(resid) + 0x1p-97 < 0x1p-53;
}

/* Correctly round x + c with x an exact f64 and c a double-double correction,
 * |c| <= |x| (the result-anchored finisher for the small-|x| odd series).
 * Boldo–Melquiond round-to-odd on the exact sticky tail.  Port of metallic-rs
 * round_anchored (mirrors round_anchored_ in asinh.c). */
static inline double sinhtab_round_anchored_(double x, exptab_sum_ c)
{
    exptab_sum_ s = exptab_twosum_(x, c.hi);
    exptab_sum_ w = exptab_twosum_(s.lo, c.lo);
    uint64_t wb = reinterpret(uint64_t, w.hi);

    if (w.lo != 0.0 && !(wb & 1)) {
        if (signbit(w.lo) == signbit(w.hi))
            wb += 1;
        else
            wb -= 1;
    }

    return s.hi + reinterpret(double, wb);
}

#endif /* METALLIC_KERNEL_SINHTAB_H */

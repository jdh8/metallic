/* Correctly-rounded accurate path for C23 `compoundn`, ported from metallic-rs
 * f64/pow_accurate.rs (compound_accurate, ~line 879): the pow cascade with a
 * log1p front end.
 *
 * log_2_/log_3_ (and p_2_/p_3_) reduce with d_mul_11_/d_mul_21_ muls that read
 * only the high limb, so they require an f64-precision base (<= 53 significant
 * bits).  The exact 1 + x has up to ~66 bits, so it cannot be fed directly.
 * Instead split 1 + x = s + c exactly (s an f64), take ln(s) through the shared
 * log, and add the high-precision correction ln(1 + c/s).  |c/s| <= 2^-53, so a
 * short series in rho = c/s -- with rho from a Newton reciprocal -- reaches the
 * phase's precision.
 *
 * The exponent reaches this file in two forms: an f64 y when (double)n is exact
 * (bit-identical to the metallic-rs pipeline the f64_compound.wc corpus was
 * certified against), or the raw int64 n when |n| > 2^53 rounds -- then the
 * exact d_mul_int_/qint_mul_int_ products replace the d_mul_21_/qint_mul_41_
 * steps with strictly smaller error, keeping CORE-MATH's phase bounds valid. */
#ifndef METALLIC_KERNEL_COMPOUNDACC_H
#define METALLIC_KERNEL_COMPOUNDACC_H

#include "powacc.h"

/* 2^-13 -- the p_2_/p_3_ series radius, below which 1 + x need not be split. */
#define COMPOUND_SPLIT_CUTOFF_ 0x1p-13

/* Reciprocal of a positive f64 as a dint (Newton r <- r(2 - ar) from the f64
 * seed; two steps reach well past the phase-2 need).  Port of recip_dint. */
static inline dint_t compound_recip_dint_(double a)
{
    dint_t qa = dint_from_f64_(a);
    dint_t two = dint_from_f64_(2.0);
    dint_t r = dint_from_f64_(1.0 / a);

    for (int i = 0; i < 2; ++i) {
        dint_t ar = dint_mul_(&qa, &r);
        ar.sgn = !ar.sgn;
        dint_t t = d_add_(&two, &ar);
        r = dint_mul_(&r, &t);
    }
    return r;
}

/* 256-bit counterpart (three Newton steps saturate the qint precision). */
static inline qint_t compound_recip_qint_(double a)
{
    qint_t qa = qint_from_f64_(a);
    qint_t two = qint_from_f64_(2.0);
    qint_t r = qint_from_f64_(1.0 / a);

    for (int i = 0; i < 3; ++i) {
        qint_t ar = qint_mul_(&qa, &r);
        ar.sgn = !ar.sgn;
        qint_t t = qint_add_(&two, &ar);
        r = qint_mul_(&r, &t);
    }
    return r;
}

/* ln(1 + x) as a dint, relative error ~2^-122 (port of log1p_2).  For
 * |x| <= 2^-13 the series keeps full accuracy however tiny x is (including
 * subnormals); otherwise the correction runs to rho^2/2 (rho^3/3 ~ 2^-159 is
 * below the phase-2 floor).  1 + x is exact for x in [-1, -1/2] (Sterbenz), so
 * c != 0 implies s >= 1/2 and the reciprocal seed cannot overflow. */
static inline dint_t compound_log1p_2_(double x)
{
    if (fabs(x) <= COMPOUND_SPLIT_CUTOFF_) {
        dint_t z = dint_from_f64_(x);
        return p_2_(&z);
    }

    double s = 1.0 + x;
    double c = x <= 1.0 ? x - (s - 1.0) : 1.0 - (s - x);
    dint_t sd = dint_from_f64_(s);
    dint_t base = log_2_(&sd);

    if (c == 0.0)
        return base; /* 1 + x is exactly representable */

    dint_t cd = dint_from_f64_(c);
    dint_t inv = compound_recip_dint_(s);
    dint_t rho = dint_mul_(&cd, &inv);
    dint_t t2 = dint_mul_(&rho, &rho);
    t2.ex -= 1;       /* rho^2/2 */
    t2.sgn = !t2.sgn; /* -rho^2/2 */
    dint_t corr = d_add_(&rho, &t2);
    return d_add_(&base, &corr);
}

/* ln(1 + x) as a qint, relative error ~2^-250 (port of log1p_3).  The
 * correction runs to rho^4/4 (rho^5/5 ~ 2^-266 is below the phase-3 floor);
 * /2 and /4 are exact exponent shifts, only /3 needs a reciprocal. */
static inline qint_t compound_log1p_3_(double x)
{
    if (fabs(x) <= COMPOUND_SPLIT_CUTOFF_) {
        qint_t z = qint_from_f64_(x);
        return p_3_(&z);
    }

    double s = 1.0 + x;
    double c = x <= 1.0 ? x - (s - 1.0) : 1.0 - (s - x);
    qint_t sq = qint_from_f64_(s);
    qint_t base = log_3_(&sq);

    if (c == 0.0)
        return base;

    qint_t cq = qint_from_f64_(c);
    qint_t inv = compound_recip_qint_(s);
    qint_t rho = qint_mul_(&cq, &inv);
    qint_t rho2 = qint_mul_(&rho, &rho);
    qint_t rho3 = qint_mul_(&rho2, &rho);
    qint_t rho4 = qint_mul_(&rho2, &rho2);

    qint_t t2 = rho2;
    t2.ex -= 1;       /* rho^2/2 */
    t2.sgn = !t2.sgn; /* -rho^2/2 */
    qint_t third = compound_recip_qint_(3.0);
    qint_t t3 = qint_mul_(&rho3, &third); /* +rho^3/3 */
    qint_t t4 = rho4;
    t4.ex -= 2;       /* rho^4/4 */
    t4.sgn = !t4.sgn; /* -rho^4/4 */

    qint_t r = qint_add_(&base, &rho);
    r = qint_add_(&r, &t2);
    r = qint_add_(&r, &t3);
    return qint_add_(&r, &t4);
}

/* Accurate (1 + x)^y for finite x > -1, x != 0, finite y not in {0, 1}: the
 * pow_accurate_ cascade with the log1p front end; the result is always
 * positive, so the sign fold is dropped.  When y_exact, y == (double)n drives
 * the same d_mul_21_/qint_mul_41_ steps as metallic-rs; otherwise the int64 n
 * multiplies exactly.  The exact/midpoint detector fires only when 1 + x is
 * itself an f64 -- an inexact 1 + x can never make (1+x)^y a short dyadic --
 * and an inexact (double)n is harmless there: exact_pow_ rejects y > 34 and a
 * power-of-two base needs x = 0. */
static inline double compound_accurate_(double x, double y, _Bool y_exact, int64_t n)
{
    /* Phase 2: dint. */
    dint_t r = compound_log1p_2_(x);

    if (y_exact) {
        dint_t big_y = dint_from_f64_(y);
        r = d_mul_21_(&r, &big_y);
    } else {
        r = d_mul_int_(&r, n);
    }
    r = exp_2_(&r);

    _Bool rd = rounding_test_2_(&r);
    r.sgn = 0; /* (1 + x)^y > 0 */
    if (rd)
        return d_tod_(&r);

    /* Exact / midpoint detection, only when 1 + x is representable. */
    double s = 1.0 + x;
    double c = x <= 1.0 ? x - (s - 1.0) : 1.0 - (s - x);
    if (c == 0.0) {
        double exact;
        if (exact_pow_(s, y, &r, &exact))
            return exact;
    }

    /* Phase 3: qint. */
    qint_t qr = compound_log1p_3_(x);

    if (y_exact) {
        qint_t qy = qint_from_f64_(y);
        qr = qint_mul_41_(&qr, &qy);
    } else {
        qr = qint_mul_int_(&qr, n);
    }
    qint_t qz = exp_3_(&qr);

    if (rounding_test_3_(&qz)) {
        qz.sgn = 0;
        qz.lo &= ~(unsigned __int128)0 << 10;
        return qint_to_f64_(qz);
    }

    /* (1 + x)^y very close to 1: |qr| < 2^-56. */
    if (qr.ex < -56) {
        double tiny = reinterpret(double, 0x3990000000000000ULL); /* 2^-100 */
        return qr.sgn ? 1.0 - tiny : 1.0 + tiny;
    }

    /* Unreachable for valid worst cases; fall back to the dint result. */
    return d_tod_(&r);
}

#endif

/* Correctly-rounded accurate path for f64 `pow`, ported from metallic-rs
 * f64/pow_accurate.rs (itself a faithful port of CORE-MATH's binary64/pow).
 *
 * Two Ziv iterations frame an exact/midpoint detector:
 *   phase 2 -- a 128-bit dint chain log_2 -> *y -> exp_2 (rel err ~2^-113),
 *   exact detection -- the rational-exponent powers x^y that are exactly
 *     representable or exact midpoints (ties-to-even cases no finite-precision
 *     approximation can decide),
 *   phase 3 -- a 256-bit qint chain log_3 -> *y -> exp_3 (rel err ~2^-240).
 *
 * metallic is round-to-nearest only, so CORE-MATH's directed-rounding and
 * errno/fenv side effects are dropped.
 *
 * The dint primitives here use CORE-MATH pow.c's add_dint/mul_dint_* semantics
 * (the d_* free functions below), which differ from the log family's dint_*
 * methods (a different reduction); only the dint_t struct, the 6-ulp dint_mul_
 * (= pow.c mul_dint) and dint_from_f64_ (= dint_fromd) are shared from dint.h. */
#ifndef METALLIC_KERNEL_POWACC_H
#define METALLIC_KERNEL_POWACC_H

#include "powtab.h" /* pulls in dint.h, powqint.h, and the constant tables */
#include <stddef.h>
#include <stdint.h>

/* === dint64_t helpers (CORE-MATH pow.c dint.h semantics) ================== */

static inline _Bool d_is_zero_(const dint_t *a)
{
    return (uint64_t)(a->m >> 64) == 0;
}

/* Compare magnitudes: -1, 0, 1 (port of cmp_dint_abs). */
static inline int d_cmp_abs_(const dint_t *a, const dint_t *b)
{
    if (d_is_zero_(a))
        return d_is_zero_(b) ? 0 : -1;
    if (d_is_zero_(b))
        return 1;
    if (a->ex != b->ex)
        return a->ex < b->ex ? -1 : 1;
    if (a->m != b->m)
        return a->m < b->m ? -1 : 1;
    return 0;
}

/* Round-to-nearest sum (port of add_dint), exact when Sterbenz applies. */
static inline dint_t d_add_(const dint_t *self, const dint_t *other)
{
    static const dint_t ZERO = { 0, 0, 0 };

    if (d_is_zero_(self))
        return *other;
    if (d_is_zero_(other))
        return *self;

    const dint_t *a, *b;
    int cmp = d_cmp_abs_(self, other);

    if (cmp == 0) {
        if (self->sgn != other->sgn)
            return ZERO;
        return (dint_t){ self->sgn, self->ex + 1, self->m };
    }
    if (cmp < 0) {
        a = other;
        b = self;
    } else {
        a = self;
        b = other;
    }

    /* |A| > |B|, so a->ex >= b->ex. */
    unsigned __int128 big_a = a->m;
    unsigned __int128 bb = b->m;
    uint32_t k = (uint32_t)(a->ex - b->ex);
    if (k > 0)
        bb = (k < 128) ? (bb >> k) : 0;

    _Bool sgn = a->sgn;
    int64_t r_ex = a->ex;
    unsigned __int128 c_out;

    if (a->sgn != b->sgn) {
        unsigned __int128 cc = big_a - bb;
        uint32_t ex = (uint32_t)dint_clz_(cc);
        if (ex > 0) {
            if (k == 1) {
                /* Sterbenz: re-subtract from the full b (low bits intact). */
                cc = (big_a << ex) - (b->m << (ex - 1));
            } else {
                cc = (big_a << ex) - (bb << ex);
            }
            r_ex -= (int64_t)ex;
            ex = (uint32_t)dint_clz_(cc);
        }
        cc <<= ex;
        r_ex -= (int64_t)ex;
        c_out = cc;
    } else {
        unsigned __int128 sum = big_a + bb;
        if (sum < big_a) {
            c_out = ((unsigned __int128)1 << 127) | (sum >> 1);
            r_ex += 1;
        } else {
            c_out = sum;
        }
    }

    return (dint_t){ sgn, r_ex, c_out };
}

/* Round-to-nearest sum assuming both low words zero (port of add_dint_11);
 * operates on the high 64-bit words only, result's low word is zero. */
static inline dint_t d_add_11_(const dint_t *self, const dint_t *other)
{
    static const dint_t ZERO = { 0, 0, 0 };

    uint64_t ahi0 = (uint64_t)(self->m >> 64);
    uint64_t bhi0 = (uint64_t)(other->m >> 64);
    if (ahi0 == 0)
        return *other;
    if (bhi0 == 0)
        return *self;

    const dint_t *a, *b;
    uint64_t ahi, bhi;
    int cmp;
    if (self->ex != other->ex)
        cmp = self->ex < other->ex ? -1 : 1;
    else if (ahi0 != bhi0)
        cmp = ahi0 < bhi0 ? -1 : 1;
    else
        cmp = 0;

    if (cmp == 0) {
        if (self->sgn != other->sgn)
            return ZERO;
        return (dint_t){ self->sgn, self->ex + 1, self->m };
    }
    if (cmp < 0) {
        a = other;
        b = self;
        ahi = bhi0;
        bhi = ahi0;
    } else {
        a = self;
        b = other;
        ahi = ahi0;
        bhi = bhi0;
    }

    uint64_t big_a = ahi, bb = bhi;
    if (a->ex > b->ex) {
        uint32_t k = (uint32_t)(a->ex - b->ex);
        bb = (k < 64) ? (bb >> k) : 0;
    }

    _Bool sgn = a->sgn;
    int64_t r_ex = a->ex;
    uint64_t cc;

    if (a->sgn != b->sgn) {
        uint64_t c0 = big_a - bb;
        uint32_t ex = c0 ? (uint32_t)__builtin_clzll(c0) : 64;
        if (ex > 0) {
            c0 = (big_a << ex) - (bb << ex);
            r_ex -= (int64_t)ex;
            ex = c0 ? (uint32_t)__builtin_clzll(c0) : 64;
        }
        cc = c0 << ex;
        r_ex -= (int64_t)ex;
    } else {
        uint64_t sum = big_a + bb;
        if (sum < big_a) {
            cc = (1ULL << 63) | (sum >> 1);
            r_ex += 1;
        } else {
            cc = sum;
        }
    }

    return (dint_t){ sgn, r_ex, (unsigned __int128)cc << 64 };
}

/* Exact product assuming both low words zero (port of mul_dint_11). */
static inline dint_t d_mul_11_(const dint_t *a, const dint_t *b)
{
    uint64_t ahi = (uint64_t)(a->m >> 64);
    uint64_t bhi = (uint64_t)(b->m >> 64);
    unsigned __int128 prod = (unsigned __int128)ahi * bhi;
    uint32_t shift = (prod >> 127) == 0;
    prod <<= shift;
    return (dint_t){ a->sgn ^ b->sgn, a->ex + b->ex + 1 - (int64_t)shift, prod };
}

/* Product assuming b's low word is zero (port of mul_dint_21), error <= 2 ulps. */
static inline dint_t d_mul_21_(const dint_t *a, const dint_t *b)
{
    unsigned __int128 ahi = (uint64_t)(a->m >> 64);
    unsigned __int128 alo = (uint64_t)a->m;
    unsigned __int128 bhi = (uint64_t)(b->m >> 64);
    unsigned __int128 hi = ahi * bhi;
    unsigned __int128 lo = alo * bhi;
    unsigned __int128 r = hi + (lo >> 64);
    uint64_t ex = (uint64_t)(r >> 127);
    r <<= 1 - ex;
    return (dint_t){ a->sgn ^ b->sgn, a->ex + b->ex + (int64_t)ex, r };
}

/* Product with a signed integer (port of mul_dint_int64), error < 1 ulp.
 * Distinct from dint_mul_int_ (mul_dint_2): no round-on-carry. */
static inline dint_t d_mul_int_(const dint_t *a, int64_t b)
{
    static const dint_t ZERO = { 0, 0, 0 };

    if (b == 0)
        return ZERO;

    uint64_t cmag = b < 0 ? -(uint64_t)b : (uint64_t)b;
    _Bool sgn = b < 0 ? !a->sgn : a->sgn;
    int64_t r_ex = a->ex + 64;

    uint64_t ahi = (uint64_t)(a->m >> 64);
    uint64_t alo = (uint64_t)a->m;
    unsigned __int128 r = (unsigned __int128)ahi * cmag;

    uint64_t rhi = (uint64_t)(r >> 64);
    uint32_t m = rhi ? (uint32_t)__builtin_clzll(rhi) : 64;
    r <<= m;
    r_ex -= (int64_t)m;

    unsigned __int128 l = (((unsigned __int128)alo * cmag) << (m - 1)) >> 63;
    unsigned __int128 sum = r + l;
    r = sum;
    if (sum < l) {
        r = ((unsigned __int128)1 << 127) | (r >> 1);
        r_ex += 1;
    }

    return (dint_t){ sgn, r_ex, r };
}

/* Truncate toward zero to an i64 (port of dint_toi). */
static inline int64_t d_toi_(const dint_t *a)
{
    if (a->ex < 0)
        return 0;
    uint64_t hi = (uint64_t)(a->m >> 64);
    int64_t r = (int64_t)(hi >> (63 - a->ex));
    return a->sgn ? -r : r;
}

/* Round-to-nearest f64, full range incl. subnormal/overflow (port of dint_tod +
 * dint_tod_subnormal, RNDN branch only). */
static inline double d_tod_(const dint_t *a)
{
    uint64_t hi = (uint64_t)(a->m >> 64);
    uint64_t lo = (uint64_t)a->m;

    if (a->ex < -1022) {
        int64_t exi = -(1011 + a->ex); /* >= 12 */
        if (exi >= 64) {
            /* |a| < 2^-1074: RNDN -> 0 or 2^-1074. */
            uint64_t rb = (exi == 64) ? (hi >> 63) : 0;
            uint64_t sb = (exi == 64) ? ((hi << 1) | lo) : (hi | lo);
            double ret = (exi > 64 || rb == 0 || sb == 0) ? 0.0 : 0x1p-1074;
            return a->sgn ? -ret : ret;
        }
        uint32_t e = (uint32_t)exi; /* 12 <= e <= 63 */
        uint64_t h = hi >> e;
        uint64_t rb = (hi >> (e - 1)) & 1;
        uint64_t sb = ((hi << (65 - e)) != 0) || (lo != 0);
        h += sb ? rb : (h & rb);
        return reinterpret(double, h | ((uint64_t)a->sgn << 63));
    }

    /* Normal range (a->ex >= -1022). */
    uint64_t r_bits = (hi >> 11) | (0x3ffULL << 52);
    double rd = 0.0;
    if ((hi >> 10) & 1)
        rd += 0x1p-53;
    if ((hi & 0x3ff) || lo)
        rd += 0x1p-54;
    if (a->sgn)
        rd = -rd;
    r_bits |= (uint64_t)a->sgn << 63;
    double r = reinterpret(double, r_bits) + rd;

    double e;
    if (a->ex > 1023) {
        if (a->ex == 1024) {
            r *= 0x1p1;
            e = 0x1p1023;
        } else {
            r = 0x1.fffffffffffffp1023;
            e = 0x1.fffffffffffffp1023;
        }
    } else {
        e = reinterpret(double, (uint64_t)((a->ex + 1023) & 0x7ff) << 52);
    }
    return r * e;
}

/* === Phase 2: 128-bit dint accurate path ================================= */

/* exp(z) for |z| < 0.00016923 as a dint (port of q_2). */
static inline dint_t q_2_(const dint_t *y)
{
    dint_t r = d_mul_11_(y, &pc_q_2[0]);
    r = d_add_11_(&pc_q_2[1], &r);
    r = d_mul_11_(y, &r);
    r = d_add_11_(&pc_q_2[2], &r);
    r = d_mul_11_(y, &r);
    r = d_add_(&pc_q_2[3], &r);
    r = dint_mul_(y, &r);
    r = d_add_(&pc_q_2[4], &r);
    r = dint_mul_(y, &r);
    r = d_add_(&pc_q_2[5], &r);
    r = dint_mul_(y, &r);
    r = d_add_(&pc_q_2[6], &r);
    r = dint_mul_(y, &r);
    return d_add_(&pc_q_2[7], &r);
}

/* log(1+z) for |z| <= 2^-13, low word of z zero (port of p_2). */
static inline dint_t p_2_(const dint_t *z)
{
    dint_t r = d_mul_11_(z, &pc_p_2[0]);
    r = d_add_11_(&pc_p_2[1], &r);
    r = d_mul_11_(z, &r);
    r = d_add_11_(&pc_p_2[2], &r);
    r = d_mul_11_(z, &r);
    r = d_add_11_(&pc_p_2[3], &r);
    r = d_mul_11_(z, &r);
    r = d_add_(&pc_p_2[4], &r);
    r = d_mul_21_(&r, z);
    r = d_add_(&pc_p_2[5], &r);
    r = d_mul_21_(&r, z);
    r = d_add_(&pc_p_2[6], &r);
    r = d_mul_21_(&r, z);
    r = d_add_(&pc_p_2[7], &r);
    r = d_mul_21_(&r, z);
    r = d_add_(&pc_p_2[8], &r);
    return d_mul_21_(&r, z);
}

/* log(x) as a dint, relative error < 2^-122.88 (port of log_2). */
static inline dint_t log_2_(const dint_t *xin)
{
    dint_t x = *xin;
    int64_t big_e = x.ex;
    uint64_t hi = (uint64_t)(x.m >> 64);
    size_t i;
    if (hi > 0xb504f333f9de6484ULL) {
        big_e += 1;
        i = (size_t)(hi >> (63 + 1 - 7));
    } else {
        i = (size_t)(hi >> (63 - 7));
    }
    /* now 90 <= i <= 181 */
    x.ex -= big_e;

    dint_t z = d_mul_11_(&x, &pc_inverse_2_1[i - 90]); /* exact */
    uint64_t zhi = (uint64_t)(z.m >> 64);
    size_t j = (size_t)(zhi >> (uint32_t)(63 - 13 - z.ex));
    z = d_mul_11_(&z, &pc_inverse_2_2[j - 8128]); /* exact */
    z = d_add_(&pc_m_one_d, &z); /* exact (subtract 1) */

    dint_t r = d_mul_int_(&pc_log2_d, big_e);
    dint_t p = p_2_(&z);
    p = d_add_(&pc_log_inv_2_2[j - 8128], &p);
    p = d_add_(&pc_log_inv_2_1[i - 90], &p);
    return d_add_(&p, &r);
}

/* exp(x) as a dint, for |x| < 744.45 (port of exp_2). */
static inline dint_t exp_2_(const dint_t *x)
{
    if (x->ex >= 10) {
        /* Underflow or overflow. */
        dint_t r = *x;
        r.ex = x->sgn ? -1076 : 1025;
        r.sgn = 0;
        return r;
    }

    dint_t big_k = d_mul_11_(x, &pc_log2_inv_d); /* exact (low word of x = 0) */
    int64_t k = d_toi_(&big_k);                  /* trunc toward zero */
    dint_t kk = d_mul_int_(&pc_log2_d, k);
    kk.ex -= 12;
    kk.sgn = !kk.sgn;
    dint_t y = d_add_(x, &kk); /* exact (Sterbenz) */

    int64_t big_m = k >> 12;
    size_t i2 = (size_t)((k >> 6) & 0x3f);
    size_t i1 = (size_t)(k & 0x3f);

    dint_t r = q_2_(&y);
    r = dint_mul_(&pc_t1_2[i2], &r);
    r = dint_mul_(&pc_t2_2[i1], &r);
    r.ex += big_m;
    return r;
}

/* Phase-2 rounding test (RNDN). */
static inline _Bool rounding_test_2_(const dint_t *r)
{
    uint64_t hi = (uint64_t)(r->m >> 64);
    uint64_t lo = (uint64_t)r->m;

    if (r->ex < -1075)
        return 1; /* underflow: rd set */
    if (r->ex < -1022) {
        uint32_t ex = (uint32_t)(-(1022 + r->ex)); /* 1 <= ex <= 53 */
        uint64_t m = (lo >> (10 + ex)) | (hi << (54 - ex));
        return (m + 14) > 28;
    }
    uint64_t lo64 = (lo >> 10) | (hi << 54);
    return (lo64 + 28) > 56;
}

/* === Phase 3: 256-bit qint accurate path ================================= */

/* exp(z) for |z| < 0.00016923 as a qint (port of q_3). */
static inline qint_t q_3_(const qint_t *y)
{
    qint_t r = qint_mul_11_(y, &pc_q_3[0]);
    r = qint_add_22_(&pc_q_3[1], &r);
    int k = 2;
    for (; k < 7; k++) {
        r = qint_mul_22_(y, &r);
        r = qint_add_22_(&pc_q_3[k], &r);
    }
    for (; k < 12; k++) {
        r = qint_mul_33_(y, &r);
        r = qint_add_(&pc_q_3[k], &r);
    }
    for (; k < 15; k++) {
        r = qint_mul_(y, &r);
        r = qint_add_(&pc_q_3[k], &r);
    }
    return r;
}

/* log(1+z) for |z| <= 2^-13, upper limb only (port of p_3). */
static inline qint_t p_3_(const qint_t *z)
{
    qint_t r = qint_mul_11_(&pc_p_3[0], z);
    r = qint_add_22_(&pc_p_3[1], &r);
    for (int k = 2; k < 4; k++) {
        r = qint_mul_11_(&r, z);
        r = qint_add_22_(&pc_p_3[k], &r);
    }
    for (int k = 4; k < 8; k++) {
        r = qint_mul_21_(&r, z);
        r = qint_add_22_(&pc_p_3[k], &r);
    }
    for (int k = 8; k < 14; k++) {
        r = qint_mul_31_(&r, z);
        r = qint_add_(&pc_p_3[k], &r);
    }
    for (int k = 14; k < 18; k++) {
        r = qint_mul_41_(&r, z);
        r = qint_add_(&pc_p_3[k], &r);
    }
    return qint_mul_41_(&r, z);
}

/* log(x) as a qint, relative error < 2^-250.74 (port of log_3). */
static inline qint_t log_3_(const qint_t *xin)
{
    qint_t x = *xin;
    int64_t big_e = x.ex;
    uint64_t hh = (uint64_t)(x.hi >> 64);
    size_t i;
    if (hh > 0xb504f333f9de6484ULL) {
        big_e += 1;
        i = (size_t)(hh >> (63 + 1 - 7));
    } else {
        i = (size_t)(hh >> (63 - 7));
    }
    x.ex -= big_e;

    qint_t z = qint_mul_(&x, &pc_inverse_3_1[i - 90]); /* exact */
    uint64_t zhh = (uint64_t)(z.hi >> 64);
    size_t j = (size_t)(zhh >> (uint32_t)(63 - 13 - z.ex));
    z = qint_mul_(&z, &pc_inverse_3_2[j - 8128]); /* exact */
    z = qint_add_(&pc_m_one_q, &z); /* exact */

    qint_t r = qint_mul_int_(&pc_log2_q, big_e);
    qint_t p = p_3_(&z);
    p = qint_add_(&pc_log_inv_3_2[j - 8128], &p);
    p = qint_add_(&pc_log_inv_3_1[i - 90], &p);
    return qint_add_(&p, &r);
}

/* exp(x) as a qint, for |x| < 744.45 (port of exp_3). */
static inline qint_t exp_3_(const qint_t *x)
{
    qint_t big_k = qint_mul_11_(x, &pc_log2_inv_q); /* exact */
    int64_t k = qint_to_i64_(&big_k);
    qint_t kk = qint_mul_int_(&pc_log2_q, k);
    kk.ex -= 12;
    kk.sgn = !kk.sgn;
    qint_t y = qint_add_(x, &kk); /* exact (Sterbenz) */

    int64_t big_m = k >> 12;
    size_t i2 = (size_t)((k >> 6) & 0x3f);
    size_t i1 = (size_t)(k & 0x3f);

    qint_t r = q_3_(&y);
    r = qint_mul_(&pc_t1_3[i2], &r);
    r = qint_mul_(&pc_t2_3[i1], &r);
    r.ex += big_m;
    return r;
}

/* Phase-3 rounding test (port of the ENABLE_ZIV3 rd computation). */
static inline _Bool rounding_test_3_(const qint_t *qz)
{
    uint64_t hh = (uint64_t)(qz->hi >> 64), hl = (uint64_t)qz->hi;
    uint64_t lh = (uint64_t)(qz->lo >> 64), ll = (uint64_t)qz->lo;
    uint64_t r1 = (hh << 54) | (hl >> 10);
    uint64_t r2 = (hl << 54) | (lh >> 10);
    uint64_t r3 = (lh << 54) | (ll >> 10);
    return !((r1 == 0 && r2 == 0 && r3 <= 60)
             || (r1 == ~0ULL && r2 == ~0ULL && (r3 + 120) <= 60));
}

/* === Exact / midpoint detection (CORE-MATH extract/pow2/round_54/exact_pow) */

/* true iff x is an integer (round-to-even). */
static inline _Bool pow_is_int_(double x)
{
    return x == rint(x);
}

/* x = 2^E * m with m odd (port of extract). */
static inline void pow_extract_(int64_t *e_out, uint64_t *m_out, double x)
{
    uint64_t u = reinterpret(uint64_t, x);
    int64_t e = (int64_t)((u >> 52) & 0x7ff);
    uint64_t m = (u & (~0ULL >> 12)) + (e ? (1ULL << 52) : 0);
    int t = __builtin_ctzll(m);
    m >>= t;
    e = e + t - (0x433 - (e == 0));
    *e_out = e;
    *m_out = m;
}

/* Multiply x by 2^e exactly when in range (port of pow2). */
static inline double pow_pow2_(double x, int64_t e)
{
    if (e & 1)
        x *= 0x1p1;
    double e2 = reinterpret(double, (((uint64_t)((e >> 1) + 0x3ff)) & 0x7ff) << 52);
    return (x * e2) * e2;
}

/* Round a dint to 54 bits assuming an all-ones / all-zeros tail (port of
 * round_54).  Returns the value as k * 2^G. */
static inline void pow_round_54_(int64_t *G, int64_t *k, const dint_t *x)
{
    uint64_t hi = (uint64_t)(x->m >> 64);
    *G = x->ex - 53;
    *k = (int64_t)((hi >> 10) + ((hi >> 9) & 1));
}

/* Detect exact / midpoint cases (port of exact_pow).  Writes the exact f64 to
 * *out and returns true when (x, y) is in CORE-MATH's set S.  z is the phase-2
 * approximation of x^y with its sign already set to the final result sign. */
static inline _Bool exact_pow_(double x, double y, const dint_t *z, double *out)
{
    _Bool neg = z->sgn;
    int64_t s_int = neg ? -1 : 1;

    int64_t big_e;
    uint64_t m;
    pow_extract_(&big_e, &m, fabs(x));

    if (m == 1) {
        /* x is a power of 2. */
        double big_g = (double)big_e * y;
        if (pow_is_int_(big_g)) {
            double r = neg ? -1.0 : 1.0;
            *out = pow_pow2_(r, (int64_t)big_g);
            return 1;
        }
        return 0;
    }

    if (y < 0.0 || y > 34.0)
        return 0;

    int64_t big_f;
    uint64_t n;
    pow_extract_(&big_f, &n, y);
    if (n > 34 || big_f < -5)
        return 0;

    if (big_f < 0) {
        /* Case (b). */
        if (((uint64_t)big_e) & (~0ULL >> (uint32_t)(64 + big_f)))
            return 0;
        int64_t g = (big_e >> (-big_f)) * (int64_t)n;

        int64_t big_g, k;
        pow_round_54_(&big_g, &k, z);
        uint32_t cnt = (uint32_t)__builtin_clzll((uint64_t)k);
        dint_t d = { !neg, big_g + 63 - (int64_t)cnt, (unsigned __int128)((uint64_t)k << cnt) << 64 };
        d = d_add_(&d, z); /* exact (Sterbenz) */
        d.ex += 116;
        if (d_cmp_abs_(&d, z) >= 0) /* |2^G k - z| >= 2^-116 z: reject */
            return 0;
        if (big_g > g)
            return 0;

        uint32_t shift = (uint32_t)(g - big_g);
        if (shift >= 64)
            return 0;
        uint64_t k_u = (uint64_t)k;
        if ((k_u & ~(~1ULL << shift)) == (1ULL << shift)) {
            double r = (double)((int64_t)(k_u >> shift) * s_int);
            *out = pow_pow2_(r, g);
            return 1;
        }
        return 0;
    }

    /* Case (a): 2 <= y <= 34 integer, y = n << F. */
    int64_t t = (int64_t)(n << big_f);
    int64_t k = 1;
    int64_t t_rem = t;
    uint64_t mm = m;
    while (t_rem != 0) {
        if (t_rem & 1) {
            unsigned __int128 v = (unsigned __int128)mm * (uint64_t)k;
            if (v > (unsigned __int128)INT64_MAX)
                return 0;
            k = (int64_t)v;
        }
        t_rem >>= 1;
        if (t_rem != 0) {
            unsigned __int128 v = (unsigned __int128)mm * mm;
            if (v > (unsigned __int128)UINT64_MAX)
                return 0;
            mm = (uint64_t)v;
        }
    }
    if ((uint64_t)k >> 54)
        return 0;
    double r = (double)(k * s_int);
    int64_t big_g = big_e * (int64_t)(n << big_f);
    *out = pow_pow2_(r, big_g);
    return 1;
}

/* === Top-level accurate path ============================================= */

/* Accurate |x|^y * s for finite positive x != 1, finite y, s = +-1.  x is
 * |base|; x0 is the original (possibly negative) base for exact_pow's 2^E*m
 * test; s is the result sign. */
static inline double pow_accurate_(double x, double y, double x0, double s)
{
    /* Phase 2: dint. */
    dint_t big_x = dint_from_f64_(x);
    big_x.sgn = 0;
    dint_t big_y = dint_from_f64_(y);

    dint_t r = log_2_(&big_x);
    r = d_mul_21_(&r, &big_y);
    r = exp_2_(&r);

    _Bool rd = rounding_test_2_(&r);
    r.sgn = s < 0.0;
    if (rd)
        return d_tod_(&r);

    /* Exact / midpoint detection (r.sgn already carries the result sign). */
    double exact;
    if (exact_pow_(x0, y, &r, &exact))
        return exact;

    /* Phase 3: qint. */
    qint_t qx = qint_from_f64_(x);
    qx.sgn = 0;
    qint_t qy = qint_from_f64_(y);

    qint_t qr = log_3_(&qx);
    qr = qint_mul_41_(&qr, &qy);
    qint_t qz = exp_3_(&qr);

    if (rounding_test_3_(&qz)) {
        qz.sgn = s < 0.0;
        qz.lo &= (~(unsigned __int128)0) << 10; /* clear low 10 bits of ll */
        return qint_to_f64_(qz);
    }

    /* x^y very close to 1: |qr| < 2^-56. */
    if (qr.ex < -56) {
        double tiny = reinterpret(double, 0x3990000000000000ULL); /* 2^-100 */
        return qr.sgn ? 1.0 - tiny : 1.0 + tiny;
    }

    /* Unreachable for valid worst cases; fall back to the dint result. */
    return d_tod_(&r);
}

#endif

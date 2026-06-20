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

#endif

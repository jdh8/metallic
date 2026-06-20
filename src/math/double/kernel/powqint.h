/* Accurate-path 256-bit fixed-point type for the correctly-rounded power
 * function (Ziv's third iteration).
 *
 * Ported from metallic-rs f64/qint.rs (itself a port of Tom Hubrecht's
 * qint64_t arithmetic in CORE-MATH binary64/pow/qint.h).  A qint_t represents
 *   value = (-1)^sgn * (M / 2^255) * 2^ex,
 * with the 256-bit significand M normalized so bit 255 is set for nonzero
 * values.  CORE-MATH stores M as four uint64 words {hh, hl, lh, ll}; here they
 * are packed into two unsigned __int128 words: hi = (hh << 64) | hl (top half),
 * lo = (lh << 64) | ll (bottom half), i.e. M = (hi << 128) | lo.
 *
 * This is exact integer arithmetic carrying ~255 significant bits: every
 * operation is plain integer add/sub/mul, no FMA.  It is the rare last-resort
 * fallback behind the 128-bit dint phase of the accurate pow path.  The to-f64
 * rounder is round-to-nearest only (the shipped WASM mode); the directed-mode
 * and errno/fenv side effects of CORE-MATH's qint_tod are dropped. */
#ifndef METALLIC_KERNEL_POWQINT_H
#define METALLIC_KERNEL_POWQINT_H

#include "../../reinterpret.h"
#include <stdint.h>

typedef struct {
    _Bool sgn;
    int64_t ex;
    unsigned __int128 hi; /* top 128 bits (hh:hl) */
    unsigned __int128 lo; /* bottom 128 bits (lh:ll) */
} qint_t;

#define QINT_LO64_ ((unsigned __int128)0xFFFFFFFFFFFFFFFFULL)

/* Count leading zeros of a 128-bit value; 128 for zero. */
static inline int qint_clz128_(unsigned __int128 x)
{
    uint64_t hi = (uint64_t)(x >> 64);
    uint64_t lo = (uint64_t)x;

    if (hi)
        return __builtin_clzll(hi);
    if (lo)
        return 64 + __builtin_clzll(lo);
    return 128;
}

/* Count leading zeros across a 256-bit value (hi:lo); 256 for zero. */
static inline int qint_clz256_(unsigned __int128 hi, unsigned __int128 lo)
{
    return hi ? qint_clz128_(hi) : 128 + qint_clz128_(lo);
}

static inline _Bool qint_is_zero_(const qint_t *a)
{
    return (uint64_t)(a->hi >> 64) == 0;
}

/* Compare magnitudes by (ex, hi, lo): -1, 0, 1. */
static inline int qint_cmp_full_(const qint_t *a, const qint_t *b)
{
    if (a->ex != b->ex)
        return a->ex < b->ex ? -1 : 1;
    if (a->hi != b->hi)
        return a->hi < b->hi ? -1 : 1;
    if (a->lo != b->lo)
        return a->lo < b->lo ? -1 : 1;
    return 0;
}

/* Compare by the upper two limbs only (ex, hi). */
static inline int qint_cmp_22_(const qint_t *a, const qint_t *b)
{
    if (a->ex != b->ex)
        return a->ex < b->ex ? -1 : 1;
    if (a->hi != b->hi)
        return a->hi < b->hi ? -1 : 1;
    return 0;
}

/* u64*u64 -> u128 partial product. */
static inline unsigned __int128 qint_p_(uint64_t x, uint64_t y)
{
    return (unsigned __int128)x * y;
}

/* a + b, returning the sum; *carry set to 0/1. */
static inline unsigned __int128 qint_addc_(unsigned __int128 a, unsigned __int128 b, unsigned __int128 *carry)
{
    unsigned __int128 s = a + b;
    *carry = s < a;
    return s;
}

/* Round-to-nearest sum, full 256-bit operands (port of add_qint). */
static inline qint_t qint_add_(const qint_t *self, const qint_t *other)
{
    static const qint_t ZERO = { 0, 0, 0, 0 };

    if (qint_is_zero_(self))
        return *other;
    if (qint_is_zero_(other))
        return *self;

    const qint_t *a, *b;
    int cmp = qint_cmp_full_(self, other);

    if (cmp == 0) {
        if (self->sgn != other->sgn)
            return ZERO;
        return (qint_t){ self->sgn, self->ex + 1, self->hi, self->lo };
    }
    if (cmp < 0) {
        a = other;
        b = self;
    } else {
        a = self;
        b = other;
    }

    /* |a| > |b|, so a->ex >= b->ex. */
    unsigned __int128 ah = a->hi, al = a->lo, bh, bl;
    int64_t m_ex = a->ex;
    uint32_t k = (uint32_t)(a->ex - b->ex);

    if (k >= 256) {
        bh = 0;
        bl = 0;
    } else if (k >= 128) {
        bl = b->hi >> (k - 128);
        bh = 0;
    } else if (k > 0) {
        bl = (b->lo >> k) | (b->hi << (128 - k));
        bh = b->hi >> k;
    } else {
        bh = b->hi;
        bl = b->lo;
    }

    _Bool sgn = a->sgn;
    int64_t r_ex = m_ex;
    unsigned __int128 ch, cl;

    if (a->sgn != b->sgn) {
        /* Subtraction C = A - B (no final borrow since |A| > |B|). */
        unsigned __int128 c_hi = ah - bh;
        unsigned __int128 c_lo = al - bl;
        if (al < bl)
            c_hi -= 1;

        uint32_t ex = qint_clz256_(c_hi, c_lo); /* < 256 since |A| > |B| */
        if (ex > 0) {
            /* Re-form the difference from the unshifted operands shifted left
             * by ex (b additionally by -k) so the leading 1 lands in bit 255. */
            if (ex >= 128) {
                ah = al << (ex - 128);
                al = 0;
            } else {
                ah = (ah << ex) | (al >> (128 - ex));
                al <<= ex;
            }
            int64_t sh = (int64_t)ex - (int64_t)k;
            bh = b->hi;
            bl = b->lo;
            if (sh >= 0) {
                uint32_t s = (uint32_t)sh;
                if (s >= 128) {
                    bh = bl << (s - 128);
                    bl = 0;
                } else if (s > 0) {
                    bh = (bh << s) | (bl >> (128 - s));
                    bl <<= s;
                }
            } else {
                uint32_t j = (uint32_t)(-sh);
                if (j >= 128) {
                    bl = bh >> (j - 128);
                    bh = 0;
                } else {
                    bl = (bh << (128 - j)) | (bl >> j);
                    bh >>= j;
                }
            }
            r_ex -= (int64_t)ex;

            unsigned __int128 c2_hi = ah - bh;
            unsigned __int128 c2_lo = al - bl;
            if (al < bl)
                c2_hi -= 1;
            ex = qint_clz256_(c2_hi, c2_lo);
            if (ex == 0) {
                ch = c2_hi;
                cl = c2_lo;
            } else if (ex >= 128) {
                ch = c2_lo << (ex - 128);
                cl = 0;
            } else {
                ch = (c2_hi << ex) | (c2_lo >> (128 - ex));
                cl = c2_lo << ex;
            }
            r_ex -= (int64_t)ex;
        } else {
            ch = c_hi;
            cl = c_lo;
        }
    } else {
        /* Addition C = A + B. */
        unsigned __int128 sum_lo = al + bl;
        _Bool carry_lo = sum_lo < al;
        unsigned __int128 sum_hi = ah + bh;
        int cy = sum_hi < ah;
        if (carry_lo) {
            unsigned __int128 s = sum_hi + 1;
            if (s < sum_hi)
                cy += 1;
            sum_hi = s;
        }
        if (cy != 0) {
            cl = (sum_hi << 127) | (sum_lo >> 1);
            ch = ((unsigned __int128)1 << 127) | (sum_hi >> 1);
            r_ex += 1;
        } else {
            ch = sum_hi;
            cl = sum_lo;
        }
    }

    return (qint_t){ sgn, r_ex, ch, cl };
}

/* Round-to-nearest sum considering only the upper two limbs (port of
 * add_qint_22); result's lower half is zero. */
static inline qint_t qint_add_22_(const qint_t *self, const qint_t *other)
{
    static const qint_t ZERO = { 0, 0, 0, 0 };

    if (qint_is_zero_(self))
        return *other;
    if (qint_is_zero_(other))
        return *self;

    const qint_t *a, *b;
    int cmp = qint_cmp_22_(self, other);

    if (cmp == 0) {
        if (self->sgn != other->sgn)
            return ZERO;
        return (qint_t){ self->sgn, self->ex + 1, self->hi, self->lo };
    }
    if (cmp < 0) {
        a = other;
        b = self;
    } else {
        a = self;
        b = other;
    }

    unsigned __int128 ah = a->hi;
    int64_t m_ex = a->ex;
    uint32_t k = (uint32_t)(a->ex - b->ex);
    unsigned __int128 bh = (k >= 128) ? 0 : (b->hi >> k);

    _Bool sgn = a->sgn;
    int64_t r_ex = m_ex;
    unsigned __int128 ch;

    if (a->sgn != b->sgn) {
        unsigned __int128 c = ah - bh;
        uint32_t ex = qint_clz128_(c);
        if (ex > 0) {
            ah <<= ex;
            bh = (ex >= k) ? (b->hi << (ex - k)) : (b->hi >> (k - ex));
            r_ex -= (int64_t)ex;
            c = ah - bh;
            ex = qint_clz128_(c);
        }
        ch = c << ex;
        r_ex -= (int64_t)ex;
    } else {
        unsigned __int128 sum = ah + bh;
        if (sum < ah) {
            ch = ((unsigned __int128)1 << 127) | (sum >> 1);
            r_ex += 1;
        } else {
            ch = sum;
        }
    }

    return (qint_t){ sgn, r_ex, ch, 0 };
}

/* Common tail of mul_qint*: normalize (t6, t5, t4) so bit 255 is set, assemble
 * the 256-bit significand, set sign/exponent.  t6 is the top 128 bits, t5 the
 * next 128, and t4's low 64 bits extend t5. */
static inline qint_t qint_finish_mul_(_Bool sgn, int64_t ex_sum,
        unsigned __int128 t6, unsigned __int128 t5, unsigned __int128 t4)
{
    uint32_t ex = (t6 >> 127) == 0;
    t5 = (t5 << 64) | (t4 & QINT_LO64_);
    unsigned __int128 hi, lo;
    if (ex == 1) {
        hi = (t6 << 1) | (t5 >> 127);
        lo = t5 << 1;
    } else {
        hi = t6;
        lo = t5;
    }
    return (qint_t){ sgn, ex_sum + 1 - (int64_t)ex, hi, lo };
}

/* Full 256x256 -> 256 product (port of mul_qint), error < 14 ulps. */
static inline qint_t qint_mul_(const qint_t *self, const qint_t *b)
{
    uint64_t ahh = (uint64_t)(self->hi >> 64), ahl = (uint64_t)self->hi;
    uint64_t alh = (uint64_t)(self->lo >> 64), all = (uint64_t)self->lo;
    uint64_t bhh = (uint64_t)(b->hi >> 64), bhl = (uint64_t)b->hi;
    uint64_t blh = (uint64_t)(b->lo >> 64), bll = (uint64_t)b->lo;

    unsigned __int128 r33 = qint_p_(ahh, bhh), r32 = qint_p_(ahh, bhl);
    unsigned __int128 r23 = qint_p_(ahl, bhh), r31 = qint_p_(ahh, blh);
    unsigned __int128 r13 = qint_p_(alh, bhh), r22 = qint_p_(ahl, bhl);
    unsigned __int128 r30 = qint_p_(ahh, bll), r03 = qint_p_(all, bhh);
    unsigned __int128 r21 = qint_p_(ahl, blh), r12 = qint_p_(alh, bhl);

    unsigned __int128 t3 = (r12 >> 64) + (r21 >> 64) + (r03 >> 64) + (r30 >> 64);
    unsigned __int128 c4, cc, c5;
    unsigned __int128 t4 = qint_addc_(r22, t3, &c4);
    t4 = qint_addc_(r13, t4, &cc);
    c4 += cc;
    t4 = qint_addc_(r31, t4, &cc);
    c4 += cc;
    unsigned __int128 t5 = qint_addc_(r23, t4 >> 64, &c5);
    t5 = qint_addc_(r32, t5, &cc);
    c5 += cc;
    unsigned __int128 t6 = r33 + ((c5 << 64) | (t5 >> 64)) + c4;

    return qint_finish_mul_(self->sgn ^ b->sgn, self->ex + b->ex, t6, t5, t4);
}

/* Product using only the upper 3 limbs of each operand (port of mul_qint_33). */
static inline qint_t qint_mul_33_(const qint_t *self, const qint_t *b)
{
    uint64_t ahh = (uint64_t)(self->hi >> 64), ahl = (uint64_t)self->hi;
    uint64_t alh = (uint64_t)(self->lo >> 64);
    uint64_t bhh = (uint64_t)(b->hi >> 64), bhl = (uint64_t)b->hi;
    uint64_t blh = (uint64_t)(b->lo >> 64);

    unsigned __int128 r33 = qint_p_(ahh, bhh), r32 = qint_p_(ahh, bhl);
    unsigned __int128 r23 = qint_p_(ahl, bhh), r31 = qint_p_(ahh, blh);
    unsigned __int128 r13 = qint_p_(alh, bhh), r22 = qint_p_(ahl, bhl);
    unsigned __int128 r21 = qint_p_(ahl, blh), r12 = qint_p_(alh, bhl);

    unsigned __int128 t3 = (r12 >> 64) + (r21 >> 64);
    unsigned __int128 c4, cc, c5;
    unsigned __int128 t4 = qint_addc_(r22, t3, &c4);
    t4 = qint_addc_(r13, t4, &cc);
    c4 += cc;
    t4 = qint_addc_(r31, t4, &cc);
    c4 += cc;
    unsigned __int128 t5 = qint_addc_(r23, t4 >> 64, &c5);
    t5 = qint_addc_(r32, t5, &cc);
    c5 += cc;
    unsigned __int128 t6 = r33 + ((c5 << 64) | (t5 >> 64)) + c4;

    return qint_finish_mul_(self->sgn ^ b->sgn, self->ex + b->ex, t6, t5, t4);
}

/* Product using only the upper limb of b (port of mul_qint_41). */
static inline qint_t qint_mul_41_(const qint_t *self, const qint_t *b)
{
    uint64_t ahh = (uint64_t)(self->hi >> 64), ahl = (uint64_t)self->hi;
    uint64_t alh = (uint64_t)(self->lo >> 64), all = (uint64_t)self->lo;
    uint64_t bhh = (uint64_t)(b->hi >> 64);

    unsigned __int128 r33 = qint_p_(ahh, bhh), r23 = qint_p_(ahl, bhh);
    unsigned __int128 r13 = qint_p_(alh, bhh), r03 = qint_p_(all, bhh);

    unsigned __int128 t3 = r03 >> 64;
    unsigned __int128 c4, c5;
    unsigned __int128 t4 = qint_addc_(r13, t3, &c4);
    unsigned __int128 t5 = qint_addc_(r23, t4 >> 64, &c5);
    unsigned __int128 t6 = r33 + ((c5 << 64) | (t5 >> 64)) + c4;

    return qint_finish_mul_(self->sgn ^ b->sgn, self->ex + b->ex, t6, t5, t4);
}

/* Exact product, upper 3 limbs of self x upper limb of b (port of mul_qint_31). */
static inline qint_t qint_mul_31_(const qint_t *self, const qint_t *b)
{
    uint64_t ahh = (uint64_t)(self->hi >> 64), ahl = (uint64_t)self->hi;
    uint64_t alh = (uint64_t)(self->lo >> 64);
    uint64_t bhh = (uint64_t)(b->hi >> 64);

    unsigned __int128 r33 = qint_p_(ahh, bhh), r23 = qint_p_(ahl, bhh);
    unsigned __int128 r13 = qint_p_(alh, bhh);

    unsigned __int128 t4 = r13;
    unsigned __int128 c5;
    unsigned __int128 t5 = qint_addc_(r23, t4 >> 64, &c5);
    unsigned __int128 t6 = r33 + ((c5 << 64) | (t5 >> 64));

    return qint_finish_mul_(self->sgn ^ b->sgn, self->ex + b->ex, t6, t5, t4);
}

/* Exact product using the upper two limbs of each operand (port of mul_qint_22). */
static inline qint_t qint_mul_22_(const qint_t *self, const qint_t *b)
{
    uint64_t ahh = (uint64_t)(self->hi >> 64), ahl = (uint64_t)self->hi;
    uint64_t bhh = (uint64_t)(b->hi >> 64), bhl = (uint64_t)b->hi;

    unsigned __int128 r33 = qint_p_(ahh, bhh), r32 = qint_p_(ahh, bhl);
    unsigned __int128 r23 = qint_p_(ahl, bhh), r22 = qint_p_(ahl, bhl);

    unsigned __int128 t4 = r22;
    unsigned __int128 c5, cc;
    unsigned __int128 t5 = qint_addc_(r23, t4 >> 64, &c5);
    t5 = qint_addc_(r32, t5, &cc);
    c5 += cc;
    unsigned __int128 t6 = r33 + ((c5 << 64) | (t5 >> 64));

    return qint_finish_mul_(self->sgn ^ b->sgn, self->ex + b->ex, t6, t5, t4);
}

/* Exact product, upper two limbs of self x upper limb of b (port of mul_qint_21). */
static inline qint_t qint_mul_21_(const qint_t *self, const qint_t *b)
{
    uint64_t ahh = (uint64_t)(self->hi >> 64), ahl = (uint64_t)self->hi;
    uint64_t bhh = (uint64_t)(b->hi >> 64);

    unsigned __int128 r33 = qint_p_(ahh, bhh), r23 = qint_p_(ahl, bhh);

    unsigned __int128 t6 = r33 + (r23 >> 64);
    unsigned __int128 t5 = r23 << 64;

    return qint_finish_mul_(self->sgn ^ b->sgn, self->ex + b->ex, t6, t5, 0);
}

/* Exact product using only the upper limb of each operand (port of mul_qint_11). */
static inline qint_t qint_mul_11_(const qint_t *self, const qint_t *b)
{
    uint64_t ahh = (uint64_t)(self->hi >> 64);
    uint64_t bhh = (uint64_t)(b->hi >> 64);
    unsigned __int128 t6 = qint_p_(ahh, bhh);
    uint32_t ex = (t6 >> 127) == 0;
    return (qint_t){ self->sgn ^ b->sgn, self->ex + b->ex + 1 - (int64_t)ex, t6 << ex, 0 };
}

/* Product with a signed integer (port of mul_qint_2), error < 2 ulps. */
static inline qint_t qint_mul_int_(const qint_t *self, int64_t b)
{
    static const qint_t ZERO = { 0, 0, 0, 0 };

    if (b == 0)
        return ZERO;

    uint64_t c = b < 0 ? -(uint64_t)b : (uint64_t)b;
    _Bool sgn = (b < 0) ^ self->sgn;
    if (c == 1)
        return (qint_t){ sgn, self->ex, self->hi, self->lo };

    int64_t r_ex = self->ex + 64;
    uint32_t k = __builtin_clzll(c);
    c <<= k;
    r_ex -= (int64_t)k;

    uint64_t ahh = (uint64_t)(self->hi >> 64), ahl = (uint64_t)self->hi;
    uint64_t alh = (uint64_t)(self->lo >> 64), all = (uint64_t)self->lo;
    unsigned __int128 cc = c;
    unsigned __int128 t3 = (unsigned __int128)ahh * cc;
    unsigned __int128 t2 = (unsigned __int128)ahl * cc;
    unsigned __int128 t1 = (unsigned __int128)alh * cc;
    unsigned __int128 t0 = (unsigned __int128)all * cc;

    unsigned __int128 t = t0 >> 64;
    unsigned __int128 cy1, t1v = qint_addc_(t, t1, &cy1);
    unsigned __int128 tmid = (cy1 << 64) | (t1v >> 64);
    unsigned __int128 cy2, t2v = qint_addc_(tmid, t2, &cy2);
    unsigned __int128 t3v = t3 + ((cy2 << 64) | (t2v >> 64));

    uint64_t t3v_hi = (uint64_t)(t3v >> 64);
    uint32_t ex = t3v_hi ? (uint32_t)__builtin_clzll(t3v_hi) : 64;
    t2v = (t2v << 64) | (t1v & QINT_LO64_);

    unsigned __int128 hi, lo;
    int64_t dex;
    if (ex != 0) {
        hi = (t3v << 1) | (t2v >> 127);
        lo = t2v << 1;
        dex = -1;
    } else {
        hi = t3v;
        lo = t2v;
        dex = 0;
    }

    return (qint_t){ sgn, r_ex + dex, hi, lo };
}

/* Convert a finite nonzero f64 to a qint_t (port of qint_fromd).  Handles both
 * normal and subnormal inputs via the clz normalization. */
static inline qint_t qint_from_f64_(double b)
{
    uint64_t bits = reinterpret(uint64_t, b);
    int64_t biased = (int64_t)((bits >> 52) & 0x7ff);
    uint64_t hh = (bits & (~0ULL >> 12)) + (biased ? (1ULL << 52) : 0);
    int64_t ex = biased - 0x3ff;
    int t = hh ? __builtin_clzll(hh) : 64;

    return (qint_t){
        b < 0.0,
        ex - (t > 11 ? t - 12 : 0),
        (unsigned __int128)(hh << t) << 64,
        0,
    };
}

/* Truncate toward zero to an i64 (port of qint_toi).  Callers keep |ex| small. */
static inline int64_t qint_to_i64_(const qint_t *a)
{
    if (a->ex < 0)
        return 0;
    uint64_t hh = (uint64_t)(a->hi >> 64);
    int64_t r = (int64_t)(hh >> (63 - a->ex));
    return a->sgn ? -r : r;
}

/* Round to f64 (round-to-nearest; port of qint_tod + subnormalize_qint, RNDN).
 * Handles the subnormal range and overflow saturation. */
static inline double qint_to_f64_(qint_t a)
{
    if (a.ex <= -1023) {
        int64_t exi = -(1011 + a.ex); /* >= 12 */
        uint64_t hh = (uint64_t)(a.hi >> 64), hl = (uint64_t)a.hi;
        uint64_t lh = (uint64_t)(a.lo >> 64), ll = (uint64_t)a.lo;

        if (exi >= 64) {
            /* |a| < 2^-1074: RNDN rounds to 0 or 2^-1074.  The midpoint
             * 2^-1075 (exi == 64, mantissa exactly 1) ties to even (0). */
            uint64_t sb = (hh << 1) | hl | lh | ll;
            double ret = (exi > 64 || sb == 0) ? 0.0 : 0x1p-1074;
            return a.sgn ? -ret : ret;
        }

        uint32_t e = (uint32_t)exi; /* 12 <= e <= 63 */
        uint64_t hi = hh >> e;
        uint64_t md = (hh >> (e - 1)) & 1;
        uint64_t sticky = (hh & (~0ULL >> e)) || hl || lh || ll;
        hi += sticky ? md : (hi & md);

        /* hi <= 2^52; hi == 2^52 (carry into the normal range) makes hi << e
         * overflow to 0, caught below — exactly the round-up-to-2^-1022 case. */
        a.hi = (unsigned __int128)(hi << e) << 64;
        a.lo = 0;
        if ((uint64_t)(a.hi >> 64) == 0) {
            a.ex += 1;
            a.hi = (unsigned __int128)(1ULL << 63) << 64;
        }
    }

    uint64_t hh = (uint64_t)(a.hi >> 64);
    uint64_t r_bits = (hh >> 11) | (0x3ffULL << 52);

    double rd = 0.0;
    if (hh & 0x400)
        rd += 0x1p-53;
    if ((hh & 0x3ff) || (uint64_t)a.hi || a.lo)
        rd += 0x1p-54;
    if (a.sgn) {
        r_bits |= 1ULL << 63;
        rd = -rd;
    }
    double r = reinterpret(double, r_bits) + rd;

    double e;
    if (a.ex > -1023) {
        if (a.ex > 1023) {
            if (a.ex == 1024) {
                r *= 0x1p1;
                e = 0x1p1023;
            } else {
                r = 0x1.fffffffffffffp1023;
                e = 0x1.fffffffffffffp1023;
            }
        } else {
            e = reinterpret(double, (uint64_t)((a.ex + 1023) & 0x7ff) << 52);
        }
    } else if (a.ex < -1074) {
        if (a.ex == -1075) {
            r *= 0x1p-1;
            e = 0x1p-1074;
        } else {
            r = 0x0.0000000000001p-1022;
            e = 0x0.0000000000001p-1022;
        }
    } else {
        e = reinterpret(double, 1ULL << (uint32_t)(a.ex + 1074));
    }

    return r * e;
}

#endif

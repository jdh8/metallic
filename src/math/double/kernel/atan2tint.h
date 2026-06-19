/* Accurate-path 192-bit fixed-point atan2 (Ziv last step).
 *
 * Ported from metallic-rs f64_/atan.rs (itself an idiomatic port of CORE-MATH
 * binary64/atan2/tint.h).  atan2's hardest hard-to-round corpus tie is ~2^-154
 * relative -- beyond the 128-bit dint path's ~2^-126 reach -- so its accurate
 * fallback needs the wider 192-bit Tint significand.  A tint_t represents
 *   value = (-1)^sgn * (hi/2^128 + lo/2^192) * 2^ex,
 * with bit 127 of hi set for nonzero values.  CORE-MATH packs the significand as
 * three uint64 words {h, m, l}; here hi = (h << 64) | m and lo = l.
 *
 * This is exact integer arithmetic carrying ~191 significant bits, so the only
 * FP touch is the final tint_to_f64_ rounding -- and that needs no software
 * fma(): the CORE-MATH fma(l, s, s*h_top) folds an exact l*s into an exact
 * s*h_top, identical to the single-rounding s*(h_top + l) used below.
 *
 * The kernel is a [29, 29] rational approximation z*P(z)/Q(z) of atan(z) on
 * [0, 1] (P/Q from rminimax, relative error ~2^-196), divided in Tint, then
 * rounded once with the error-bound-aware tint_to_f64_.  Constants P_TINT,
 * Q_TINT, PI2_TINT, PI_TINT are copied verbatim from CORE-MATH atan2.c (the
 * arrays were generated mechanically from atan.rs; do not edit by hand). */
#ifndef METALLIC_KERNEL_ATAN2TINT_H
#define METALLIC_KERNEL_ATAN2TINT_H

#include "../../reinterpret.h"
#include <math.h>
#include <stdint.h>

typedef struct {
    _Bool sgn;
    int64_t ex;
    unsigned __int128 hi;
    uint64_t lo;
} tint_t;

/* Significand pair returned by the shift helpers. */
typedef struct {
    unsigned __int128 hi;
    uint64_t lo;
} tint_sig_;

static const tint_t tint_zero_ = { 0, -1076, 0, 0 };
static const tint_t tint_one_  = { 0, 1, (unsigned __int128)0x8000000000000000ULL << 64, 0 };
static const tint_t tint_pi2_  = { 0, 1,
    ((unsigned __int128)0xc90fdaa22168c234ULL << 64) | 0xc4c6628b80dc1cd1ULL, 0x29024e088a67cc74ULL };
static const tint_t tint_pi_   = { 0, 2,
    ((unsigned __int128)0xc90fdaa22168c234ULL << 64) | 0xc4c6628b80dc1cd1ULL, 0x29024e088a67cc74ULL };

/* Leading-zero count of the 192-bit significand (hi, lo); 192 for zero. */
static inline uint32_t tint_clz192_(unsigned __int128 hi, uint64_t lo)
{
    if (hi) {
        uint64_t top = (uint64_t)(hi >> 64);
        return top ? (uint32_t)__builtin_clzll(top)
                   : 64 + (uint32_t)__builtin_clzll((uint64_t)hi);
    }
    return lo ? 128 + (uint32_t)__builtin_clzll(lo) : 192;
}

/* Compare magnitudes: -1, 0, 1 for |a| < |b|, ==, > (port of cmp_tint_abs). */
static inline int tint_cmp_abs_(const tint_t *a, const tint_t *b)
{
    int az = a->hi == 0, bz = b->hi == 0;

    if (az)
        return bz ? 0 : -1;
    if (bz)
        return 1;
    if (a->ex != b->ex)
        return a->ex < b->ex ? -1 : 1;
    if (a->hi != b->hi)
        return a->hi < b->hi ? -1 : 1;
    if (a->lo != b->lo)
        return a->lo < b->lo ? -1 : 1;
    return 0;
}

/* Significand shifted right by k bits (port of rshift). */
static inline tint_sig_ tint_rshift_(const tint_t *t, uint32_t k)
{
    tint_sig_ r;

    if (k == 0) {
        r.hi = t->hi;
        r.lo = t->lo;
    } else if (k < 64) {
        r.hi = t->hi >> k;
        r.lo = (uint64_t)(t->hi << (64 - k)) | (t->lo >> k);
    } else if (k == 64) {
        r.hi = t->hi >> 64;
        r.lo = (uint64_t)t->hi;
    } else if (k < 128) {
        r.hi = t->hi >> k;
        r.lo = (uint64_t)(t->hi >> (k - 64));
    } else if (k < 192) {
        r.hi = 0;
        r.lo = (uint64_t)(t->hi >> (k - 64));
    } else {
        r.hi = 0;
        r.lo = 0;
    }
    return r;
}

/* Significand shifted left by k bits (port of lshift). */
static inline tint_sig_ tint_lshift_(const tint_t *t, uint32_t k)
{
    tint_sig_ r;

    if (k == 0) {
        r.hi = t->hi;
        r.lo = t->lo;
    } else if (k < 64) {
        r.hi = (t->hi << k) | (unsigned __int128)(t->lo >> (64 - k));
        r.lo = t->lo << k;
    } else if (k == 64) {
        r.hi = (t->hi << 64) | (unsigned __int128)t->lo;
        r.lo = 0;
    } else if (k < 128) {
        r.hi = (t->hi << k) | ((unsigned __int128)t->lo << (k - 64));
        r.lo = 0;
    } else if (k < 192) {
        r.hi = (unsigned __int128)t->lo << (k - 64);
        r.lo = 0;
    } else {
        r.hi = 0;
        r.lo = 0;
    }
    return r;
}

/* Negate (flip the sign bit). */
static inline tint_t tint_neg_(tint_t t)
{
    t.sgn = !t.sgn;
    return t;
}

/* Round-to-nearest product, error < 10 ulp (port of mul_tint). */
static inline tint_t tint_mul_(const tint_t *self, const tint_t *other)
{
    unsigned __int128 ah = (uint64_t)(self->hi >> 64);
    unsigned __int128 am = (uint64_t)self->hi;
    unsigned __int128 al = self->lo;
    unsigned __int128 bh = (uint64_t)(other->hi >> 64);
    unsigned __int128 bm = (uint64_t)other->hi;
    unsigned __int128 bl = other->lo;

    unsigned __int128 rh = ah * bh;
    unsigned __int128 rm1 = ah * bm;
    unsigned __int128 rm2 = am * bh;
    unsigned __int128 rl1 = ah * bl;
    unsigned __int128 rl2 = am * bm;
    unsigned __int128 rl3 = al * bh;

    uint64_t r_h = (uint64_t)(rh >> 64);
    uint64_t r_m = (uint64_t)rh;
    uint64_t r_l = (uint64_t)rm1;
    uint64_t nm, nl, c, c0, h, l, cm;

    /* accumulate rm1 */
    h = (uint64_t)(rm1 >> 64);
    nm = r_m + h; c = nm < r_m; r_m = nm; r_h += c;
    /* accumulate rm2 */
    l = (uint64_t)rm2; h = (uint64_t)(rm2 >> 64);
    nl = r_l + l; c0 = nl < r_l; r_l = nl; cm = c0;
    nm = r_m + h; c = nm < r_m; r_m = nm; r_h += c;
    /* accumulate the high words of rl1 + rl2 + rl3 */
    unsigned __int128 rl = (rl1 >> 64) + (rl2 >> 64) + (rl3 >> 64);
    l = (uint64_t)rl;
    cm += (uint64_t)(rl >> 64);
    nl = r_l + l; c0 = nl < r_l; r_l = nl; cm += c0;
    /* accumulate cm */
    nm = r_m + cm; c = nm < r_m; r_m = nm; r_h += c;

    unsigned __int128 hi = ((unsigned __int128)r_h << 64) | r_m;
    uint64_t lo = r_l;
    int64_t ex = self->ex + other->ex;

    /* normalize so bit 127 is set (unless zero) */
    if ((hi >> 127) == 0) {
        hi = (hi << 1) | (lo >> 63);
        lo <<= 1;
        ex -= 1;
    }

    tint_t r = { self->sgn ^ other->sgn, ex, hi, lo };
    return r;
}

/* Round-to-nearest sum, error <= 2 ulp (port of add_tint). */
static inline tint_t tint_add_(const tint_t *self, const tint_t *other)
{
    int cmp = tint_cmp_abs_(self, other);
    const tint_t *a, *b;

    if (cmp == 0) {
        /* |a| = |b|: 0 (opposite signs) or 2*a (same sign), exact. */
        if (self->sgn != other->sgn)
            return tint_zero_;
        tint_t r = *self;
        r.ex = self->ex + 1;
        return r;
    }
    if (cmp < 0) { a = other; b = self; }
    else         { a = self; b = other; }

    uint32_t sh = (uint32_t)(a->ex - b->ex);
    tint_sig_ t = tint_rshift_(b, sh);

    if (a->sgn != b->sgn) {
        /* opposite signs: subtraction a - (shifted b) */
        uint64_t l = a->lo - t.lo;
        uint64_t borrow0 = a->lo < t.lo;
        unsigned __int128 hh = a->hi - t.hi - (unsigned __int128)borrow0;
        uint32_t ex = tint_clz192_(hh, l);

        if (ex <= 1 || sh == 0) {
            tint_t tmp = { 0, 0, hh, l };
            tint_sig_ r = tint_lshift_(&tmp, ex);
            tint_t out = { a->sgn, a->ex - (int64_t)ex, r.hi, r.lo };
            return out;
        }
        /* ex >= 2 and sh >= 1: redo with no neglected bits of b. */
        tint_sig_ tb = tint_lshift_(b, ex - sh);
        tint_sig_ ra = tint_lshift_(a, ex);
        uint64_t l2 = ra.lo - tb.lo;
        uint64_t borrow = ra.lo < tb.lo;
        unsigned __int128 h2 = ra.hi - tb.hi - (unsigned __int128)borrow;
        uint32_t ex1 = tint_clz192_(h2, l2);
        tint_t tmp = { 0, 0, h2, l2 };
        tint_sig_ r = tint_lshift_(&tmp, ex1);
        tint_t out = { a->sgn, a->ex - (int64_t)(ex + ex1), r.hi, r.lo };
        return out;
    }

    /* same signs: addition */
    uint64_t l = a->lo + t.lo;
    uint64_t cl = l < a->lo;
    unsigned __int128 hh = a->hi + t.hi;
    unsigned __int128 ch = hh < a->hi;
    unsigned __int128 h2 = hh + (unsigned __int128)cl;
    ch += h2 < hh;
    hh = h2;

    if (ch != 0) {
        /* carry out of bit 127: shift right one, restoring the carry bit. */
        uint64_t lo = ((uint64_t)hh << 63) | (l >> 1);
        unsigned __int128 hi = (ch << 127) | (hh >> 1);
        tint_t out = { a->sgn, a->ex + 1, hi, lo };
        return out;
    }
    tint_t out = { a->sgn, a->ex, hh, l };
    return out;
}

/* Convert a finite nonzero f64 to a tint_t, exactly (port of tint_fromd). */
static inline tint_t tint_from_f64_(double x)
{
    uint64_t u = reinterpret(uint64_t, x);
    _Bool sgn = u >> 63;
    uint64_t ax = u & 0x7fffffffffffffffULL;
    int64_t e = ax >> 52;
    int64_t ex;
    uint64_t h;

    if (e != 0) {
        ex = e - 0x3fe;
        h = (1ULL << 63) | (ax << 11);   /* leaked exponent bits land on bit 63 */
    } else {
        int lz = __builtin_clzll(ax);
        ex = -0x3f2 - lz;
        h = ax << lz;
    }

    tint_t r = { sgn, ex, (unsigned __int128)h << 64, 0 };
    return r;
}

/* Convert to f64 with correct rounding (port of tint_tod).  err is a bound in
 * ulps of 2^-192 on lo; err == 0 means exact. */
static inline double tint_to_f64_(const tint_t *self, uint64_t err)
{
    if (self->ex >= 1025)
        return self->sgn ? -INFINITY : INFINITY;

    if (self->ex <= -1074) {
        if (self->ex < -1074)
            return (self->sgn ? -1.0 : 1.0) * 0x1p-1074 * 0.5;
        int mid = self->hi == ((unsigned __int128)1 << 127) && self->lo == 0;
        return (self->sgn ? -1.0 : 1.0) * 0x1p-1074 * (mid ? 0.5 : 0.75);
    }

    uint64_t hh = (uint64_t)(self->hi >> 64);
    uint64_t mm = (uint64_t)self->hi;
    uint64_t ll = self->lo;
    int64_t ex = self->ex;
    uint64_t low = hh & 0x7ff;

    if (ex <= -1022) {
        /* subnormal result: shift the significand right by sh in 1..=52. */
        uint32_t sh = (uint32_t)(-1021 - ex);
        ll = (mm << (64 - sh)) | (ll >> sh) | (uint64_t)(ll > 0);
        mm = (hh << (64 - sh)) | (mm >> sh);
        hh >>= sh;
        low = hh & 0x7ff;
        ex += (int64_t)sh;
    }

    double h_top = (double)(hh >> 11);   /* top 53 bits */
    double l;

    if (err == 0)            l = 0.0;
    else if (low < 0x400)    l = 0.25;   /* round toward zero */
    else if (low > 0x400)    l = 0.75;   /* round away */
    else if (mm == 0 && ll == 0) l = 0.5;/* exact midpoint */
    else                     l = 0.75;   /* round away */

    double s = self->sgn ? -1.0 : 1.0;
    /* fma(l, s, s*h_top): both products exact, so s*(h_top + l) rounds once. */
    double h = s * (h_top + l);
    h *= 0x1p-52;
    /* -1021 <= ex <= 1024, so 2^(ex-1) neither under- nor overflows. */
    return h * scalbn(1.0, (int)(ex - 1));
}

/* Approximate 1/self, relative error <= 2^-103.9 (port of inv_tint). */
static inline tint_t tint_inv_(const tint_t *self)
{
    double a = tint_to_f64_(self, 0);   /* exact */
    int subnormal = fabs(a) < 0x1p-1022;

    if (subnormal)
        a *= 0x1p53;

    tint_t r = tint_from_f64_(1.0 / a);

    if (subnormal)
        r.ex += 53;

    /* Newton: r <- r + r*(1 - a*r). */
    tint_t q = tint_mul_(self, &r);
    q.sgn = !q.sgn;                     /* -a*r */
    q = tint_add_(&tint_one_, &q);      /* 1 - a*r */
    q = tint_mul_(&r, &q);              /* r*(1 - a*r) */
    return tint_add_(&r, &q);
}

/* Approximate b/a, relative error <= 2^-185.53 (port of div_tint). */
static inline tint_t tint_div_(const tint_t *b, const tint_t *a)
{
    tint_t y = tint_inv_(a);            /* |y - 1/a| small */
    tint_t r = tint_mul_(&y, b);        /* r ~ b/a */
    /* Karp-Markstein: r <- r + y*(b - a*r). */
    tint_t z = tint_mul_(a, &r);        /* a*r */
    z.sgn = !z.sgn;                     /* -a*r */
    z = tint_add_(b, &z);               /* b - a*r */
    z = tint_mul_(&y, &z);              /* y*(b - a*r) */
    return tint_add_(&r, &z);
}

/* b / a for f64 inputs (port of div_tint_d). */
static inline tint_t tint_div_d_(double b, double a)
{
    tint_t bt = tint_from_f64_(b);
    tint_t at = tint_from_f64_(a);
    return tint_div_(&bt, &at);
}

/* P(z) coefficients of the [29, 29] atan rational, low-degree first -- copied
 * verbatim from CORE-MATH atan2.c (generated from atan.rs, do not hand-edit). */
static const tint_t atan2_p_tint_[30] = {
    { 0, -10, ((unsigned __int128)0x82703f8b53112ecaULL << 64) | 0x71ce6279d467aa45ULL, 0x019c9b28466b6fc0ULL },
    { 0, -8, ((unsigned __int128)0xb3cf74b427d53e03ULL << 64) | 0xee590b839e751890ULL, 0xf2570b12bcb2381aULL },
    { 0, -6, ((unsigned __int128)0xb08498b17b88c39aULL << 64) | 0x11e37821d784d8faULL, 0x2795b14b84b9d78cULL },
    { 0, -5, ((unsigned __int128)0xfd4baa7bb52b0d83ULL << 64) | 0xb25425beb0a04dc3ULL, 0x92008d9fb34b68ffULL },
    { 0, -3, ((unsigned __int128)0x96541948b85fb386ULL << 64) | 0xe0b815ace425a859ULL, 0xbeca17c69bb4f7a0ULL },
    { 0, -2, ((unsigned __int128)0x96aaa5d4baffc590ULL << 64) | 0xf23db783b6e4ce55ULL, 0x2c54e054d3b69f7eULL },
    { 0, -1, ((unsigned __int128)0x83cd41b5fa862b11ULL << 64) | 0xad8f9d6e3eb9d833ULL, 0x4ef470d801fa03caULL },
    { 0, -1, ((unsigned __int128)0xcbbc7752197cc106ULL << 64) | 0xbc66cfb3edefa6e3ULL, 0x568b2f9e4feb8910ULL },
    { 0, 0, ((unsigned __int128)0x8cf8f7c585767042ULL << 64) | 0xcd92a5b9ac13d8c8ULL, 0x043bd4a023dbe04dULL },
    { 0, 0, ((unsigned __int128)0xafdc36d6b36139bfULL << 64) | 0x11df63474e3859d4ULL, 0x42b30b2c089a2b6cULL },
    { 0, 0, ((unsigned __int128)0xc6f8fb1463822519ULL << 64) | 0x2f9f04cf55c10377ULL, 0x1c44675c6d483606ULL },
    { 0, 0, ((unsigned __int128)0xcce79f10f754e315ULL << 64) | 0x91ce47edcbc8a4eeULL, 0xb2a6f721ac656b42ULL },
    { 0, 0, ((unsigned __int128)0xc090384dbd77132eULL << 64) | 0x1dfb4d9a1d595de7ULL, 0xe506ca899a394f70ULL },
    { 0, 0, ((unsigned __int128)0xa556ac9eee25e867ULL << 64) | 0xd84dafa25141d203ULL, 0xf1c7c2a5156b3ec3ULL },
    { 0, 0, ((unsigned __int128)0x81bd2677ddfcbc38ULL << 64) | 0x1df33f27dbb60939ULL, 0xcb4c2c73286a6c29ULL },
    { 0, -1, ((unsigned __int128)0xb9ed12169d72e8d9ULL << 64) | 0xc81d95dcef13c227ULL, 0x974b95796b93c332ULL },
    { 0, -2, ((unsigned __int128)0xf2df6da875b666faULL << 64) | 0xc29dd3a69c274c65ULL, 0x83b9260976c8f0c5ULL },
    { 0, -2, ((unsigned __int128)0x902d34d4db910ed8ULL << 64) | 0x6f3ecddfd6af0a9fULL, 0x8e950d7e70a8e9b2ULL },
    { 0, -3, ((unsigned __int128)0x9aefa41961ce1207ULL << 64) | 0xacd0317666753751ULL, 0x3eaf42a53fcc6534ULL },
    { 0, -4, ((unsigned __int128)0x95db9977c5e6e263ULL << 64) | 0x85367a63fab8a720ULL, 0xfc2b63bd115998d8ULL },
    { 0, -5, ((unsigned __int128)0x817cb6d8236d5f4dULL << 64) | 0x70c7815f28909d9bULL, 0x0c54e97155d12158ULL },
    { 0, -7, ((unsigned __int128)0xc5eec4228c352098ULL << 64) | 0x789e56be1dee9b67ULL, 0xba25bda81292ccc1ULL },
    { 0, -8, ((unsigned __int128)0x840c38ccfcb5dc64ULL << 64) | 0xb55a44095bf7f312ULL, 0x6668b8792d4ecd08ULL },
    { 0, -10, ((unsigned __int128)0x97074af7b286b19eULL << 64) | 0xc20aac21f3130f1dULL, 0x369e9fdbdd48528aULL },
    { 0, -12, ((unsigned __int128)0x9054011c32d4b7a5ULL << 64) | 0xc68d3c5d525f4af4ULL, 0xeeb3c98615dee188ULL },
    { 0, -15, ((unsigned __int128)0xdddb4a2eaab2bb15ULL << 64) | 0x753761a8f0e9aa8dULL, 0x3f20620cd3e075bbULL },
    { 0, -17, ((unsigned __int128)0x80ef9a1b3e59abe5ULL << 64) | 0x18e6afe0651fdf49ULL, 0xd1c0329e6a702d9dULL },
    { 0, -21, ((unsigned __int128)0xca16dd49cd30854cULL << 64) | 0xbad8ca379401027cULL, 0x1fc7f47985486f16ULL },
    { 0, -25, ((unsigned __int128)0xa0ba01b8742ba801ULL << 64) | 0xe6dc14a9516d7f04ULL, 0x1f5303d878905d2fULL },
    { 1, -59, ((unsigned __int128)0x9c0440896ce6515cULL << 64) | 0x0cf3470fa7f10ac3ULL, 0xa9dc80682c111a9dULL },
};

/* Q(z) coefficients of the [29, 29] atan rational, low-degree first. */
static const tint_t atan2_q_tint_[30] = {
    { 0, -10, ((unsigned __int128)0x82703f8b53112ecaULL << 64) | 0x71ce6279d467aa45ULL, 0x019c9b28466b6fc0ULL },
    { 0, -8, ((unsigned __int128)0xb3cf74b427d53e03ULL << 64) | 0xee590b839e751890ULL, 0xf2570b12bcb238cfULL },
    { 0, -6, ((unsigned __int128)0xb33c44af0d43c9e8ULL << 64) | 0xf4426f84614701dbULL, 0x979e49dc5b858442ULL },
    { 0, -4, ((unsigned __int128)0x8264d256f0bf4d61ULL << 64) | 0xee21431cc0f29764ULL, 0xce0cc1b593ac9952ULL },
    { 0, -3, ((unsigned __int128)0x9d97c4947687b2eeULL << 64) | 0xef4087c49dde5229ULL, 0x742bc6e02bfb6df8ULL },
    { 0, -2, ((unsigned __int128)0xa0f888dc7aab949eULL << 64) | 0xf623f19ac413db23ULL, 0x854744a164f70ecdULL },
    { 0, -1, ((unsigned __int128)0x8fd9c7172ab0828eULL << 64) | 0x1a9f72fadafc3dfbULL, 0xaf96a180ea297178ULL },
    { 0, -1, ((unsigned __int128)0xe38168b78a51ee15ULL << 64) | 0x604a47815c8091ebULL, 0x9faaa42312e8eb30ULL },
    { 0, 0, ((unsigned __int128)0xa164ccdbb65ee133ULL << 64) | 0xe623433c5e086402ULL, 0xcb228afd335a245fULL },
    { 0, 0, ((unsigned __int128)0xced0c0cbe44fac2fULL << 64) | 0xd7c2adfc7c5dda9eULL, 0xa22b4a311cf5a5b1ULL },
    { 0, 0, ((unsigned __int128)0xf0e6660438c90371ULL << 64) | 0x3dca1a0f58f0d943ULL, 0x8a92b9de68c4488cULL },
    { 0, 1, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL, 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xf8ea68f9ad849103ULL << 64) | 0xb257e4bfc606967fULL, 0x818fa2816928c4aaULL },
    { 0, 0, ((unsigned __int128)0xddc64eeee3506234ULL << 64) | 0x47a33ea40a2b22e4ULL, 0xa6a2c129ace2470aULL },
    { 0, 0, ((unsigned __int128)0xb52ddfba2d37c8e2ULL << 64) | 0x24551a9b19fa064aULL, 0x3d78c68b54bc3850ULL },
    { 0, 0, ((unsigned __int128)0x87ac113d26b683a1ULL << 64) | 0x4e97756c38785b0bULL, 0xf75f0a4506931ba8ULL },
    { 0, -1, ((unsigned __int128)0xba065070b900f3d5ULL << 64) | 0xe49d8f9f238f1eebULL, 0x6805d48d67f1dcd9ULL },
    { 0, -2, ((unsigned __int128)0xe902d2a4bfc22441ULL << 64) | 0x945cfe6c83a5b828ULL, 0x519ebb457f8ccca0ULL },
    { 0, -2, ((unsigned __int128)0x84e405c915f7769bULL << 64) | 0x0faaf34c3c51dd39ULL, 0xa97fa0ae99382c93ULL },
    { 0, -3, ((unsigned __int128)0x896da46c7e6cebb3ULL << 64) | 0xe0afc0b6db3dbbf5ULL, 0x061ccff6e78efff4ULL },
    { 0, -4, ((unsigned __int128)0x801a0c1a0dfefc92ULL << 64) | 0xf8e0a97902e08d47ULL, 0x81898e0b7d8ef0bcULL },
    { 0, -6, ((unsigned __int128)0xd59d088930df07dbULL << 64) | 0xa8cb00021849fbafULL, 0xf80cc91aa0bddf89ULL },
    { 0, -7, ((unsigned __int128)0x9db56a6984b77231ULL << 64) | 0xcf7c2074e0480959ULL, 0x3316ecaa6aa4604eULL },
    { 0, -9, ((unsigned __int128)0xcb7450d9d62d2e68ULL << 64) | 0x9787b4265977d004ULL, 0xba615cee00a841d7ULL },
    { 0, -11, ((unsigned __int128)0xe12a52e02b64b23eULL << 64) | 0x97c9139a8ced716cULL, 0x70a176ad2b5f16d2ULL },
    { 0, -13, ((unsigned __int128)0xd057b6d0900ed30dULL << 64) | 0x5e04bb17eb879a5eULL, 0x23ed135b60bea070ULL },
    { 0, -15, ((unsigned __int128)0x9b1f1ca22fef7ae5ULL << 64) | 0x19a687f0a4073eeeULL, 0x05a9ccbc8c14578bULL },
    { 0, -18, ((unsigned __int128)0xaeba710f053aac3bULL << 64) | 0x30ea5d6fb116f775ULL, 0x8a9bcfef5698f8f1ULL },
    { 0, -21, ((unsigned __int128)0x84b9ada076cd007dULL << 64) | 0x13e55006073f76b1ULL, 0x4cf4259968228f3eULL },
    { 0, -26, ((unsigned __int128)0xcca4b9046786937dULL << 64) | 0x233c1e691c127c1fULL, 0xf493e0b7a3de459aULL },
};

/* atan2's magnitude in [0, pi] for finite nonzero a = |x|, b = |y|, as a
 * correctly-rounded f64 -- the 192-bit accurate fallback (port of CORE-MATH's
 * atan2_accurate / metallic-rs atan2_tint_mag).  The NaN/inf/zero/axis and
 * fully-underflowed special cases are handled by the caller before the Ziv gate
 * escalates here. */
static inline double atan2_tint_mag_(double a, double b, _Bool x_negative)
{
    /* inv = |y| > |x| (CORE-MATH's `inv`); either way z = small/big in [0, 1]. */
    _Bool inv = b > a;
    double num = inv ? a : b;
    double den = inv ? b : a;
    tint_t z = tint_div_d_(num, den);

    /* Tiny ratio with the larger leg as x: atan(z) rounds to z nudged a hair
     * toward zero (CORE-MATH's `z->l -= 2; tint_tod(z, 1)`).  Only valid when
     * the result IS that ratio, i.e. x > 0 with no pi fold -- in metallic-rs
     * the fast path absorbs x < 0 tiny ratios, but this C path escalates every
     * input here, so guard `!x_negative` explicitly (else x < 0 returns ~0
     * instead of ~pi). */
    if (!inv && !x_negative && z.ex <= -96) {
        uint64_t borrow = z.lo < 2;
        z.lo -= 2;
        z.hi -= (unsigned __int128)borrow;
        return tint_to_f64_(&z, 1);
    }

    /* z*P(z)/Q(z): the [29, 29] rational approximation of atan on [0, 1]. */
    tint_t p = atan2_p_tint_[29];
    tint_t q = atan2_q_tint_[29];

    for (int i = 28; i >= 0; --i) {
        p = tint_mul_(&p, &z);
        q = tint_mul_(&q, &z);
        p = tint_add_(&p, &atan2_p_tint_[i]);
        q = tint_add_(&q, &atan2_q_tint_[i]);
    }
    p = tint_mul_(&p, &z);              /* p <- z*P(z) */
    z = tint_div_(&p, &q);              /* z <- atan(small/big) in [0, pi/4] */

    /* phi = atan(small/big), or pi/2 - phi when swapped (inv); then x < 0 gives
     * the opposite quadrant theta = pi - phi.  a, b >= 0 keeps z >= 0.  err is
     * the ulp bound CORE-MATH attaches to the last applied fold. */
    if (inv) {
        tint_t nz = tint_neg_(z);
        z = tint_add_(&tint_pi2_, &nz);
    }
    if (x_negative) {
        tint_t nz = tint_neg_(z);
        z = tint_add_(&tint_pi_, &nz);
    }

    uint64_t err = x_negative ? 266 : inv ? 524 : 662;
    return tint_to_f64_(&z, err);
}

#endif

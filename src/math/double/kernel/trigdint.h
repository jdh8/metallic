/* Correctly-rounded double sin/cos/tan — accurate (128-bit Dint) path.
 *
 * Ported from metallic-rs (src/f64_/trig.rs).  Reached only on the Ziv
 * fallback: re-reduce x to a 128-bit reduced angle whose residual fraction is
 * read out *normalized* (full 128-bit relative precision even when x is very
 * near a multiple of pi/2, which the double-double path would lose), evaluate
 * the Taylor kernels in Dint (~2^-126), and round once with dint_to_f64_.
 *
 * The Dint type and arithmetic (dint_add_, dint_mul_, dint_from_f64_,
 * dint_to_f64_, dint_clz_) live in dint.h; trig.h supplies TRIG_FRAC_2_PI.
 */
#ifndef METALLIC_KERNEL_TRIGDINT_H
#define METALLIC_KERNEL_TRIGDINT_H

#include "dint.h"
#include "trig.h"

#define TRIG_U128(hi, lo) (((unsigned __int128)UINT64_C(hi) << 64) | UINT64_C(lo))

static const dint_t PIO2_DINT = { 0, 0, TRIG_U128(0xc90fdaa22168c234, 0xc4c6628b80dc1cd1) };
static const dint_t TWO_DINT = { 0, 1, TRIG_U128(0x8000000000000000, 0x0000000000000000) };

/* sin(r)/r as Dint minimax coefficients in u = r^2, low-degree first. */
static const dint_t SIN_DINT[14] = {
    { 0, 0, TRIG_U128(0x8000000000000000, 0x0000000000000000) },
    { 1, -3, TRIG_U128(0xaaaaaaaaaaaaaaaa, 0xaaaaaaaaaaaaaaaa) },
    { 0, -7, TRIG_U128(0x8888888888888888, 0x8888888888888304) },
    { 1, -13, TRIG_U128(0xd00d00d00d00d00d, 0x00d00d00cfd3c095) },
    { 0, -19, TRIG_U128(0xb8ef1d2ab6399c7d, 0x560e44714a1cf810) },
    { 1, -26, TRIG_U128(0xd7322b3faa271c7f, 0x3a3f1de7e8537244) },
    { 0, -33, TRIG_U128(0xb092309d43684be5, 0x1bf8946b51ffd81a) },
    { 1, -41, TRIG_U128(0xd73f9f399dc0f88e, 0x072a304aab268f7a) },
    { 0, -49, TRIG_U128(0xca963b81856a506e, 0x6987c48d14c9f8fc) },
    { 1, -57, TRIG_U128(0x97a4da340a028ffc, 0x77b0b12c8f37e854) },
    { 0, -66, TRIG_U128(0xb8dc77b6c7a286d6, 0x23f9a0ebf12f4b58) },
    { 1, -75, TRIG_U128(0xbb0da042577331ee, 0x2548b4a4a2bba83e) },
    { 0, -84, TRIG_U128(0x9f9dceae8bd1c380, 0x7edcb35637352631) },
    { 1, -94, TRIG_U128(0xe79967c7c1fd457e, 0x8bad5620ab5c78a2) },
};

/* cos(r) as Dint minimax coefficients in u = r^2, low-degree first. */
static const dint_t COS_DINT[14] = {
    { 0, 0, TRIG_U128(0x8000000000000000, 0x0000000000000000) },
    { 1, -2, TRIG_U128(0xffffffffffffffff, 0xfffffffffffffff4) },
    { 0, -5, TRIG_U128(0xaaaaaaaaaaaaaaaa, 0xaaaaaaaaaaaa82ad) },
    { 1, -10, TRIG_U128(0xb60b60b60b60b60b, 0x60b60b60b53be8cd) },
    { 0, -16, TRIG_U128(0xd00d00d00d00d00d, 0x00d00cfc6ce73cc3) },
    { 1, -22, TRIG_U128(0x93f27dbbc4fae397, 0x780b5bbb83a5f0ac) },
    { 0, -29, TRIG_U128(0x8f76c77fc6c4bdaa, 0x269903347e0ad492) },
    { 1, -37, TRIG_U128(0xc9cba54603e4e904, 0x825156e42e86c60a) },
    { 0, -45, TRIG_U128(0xd73f9f399dc0f345, 0xa3a45aadb465b2fa) },
    { 1, -53, TRIG_U128(0xb413c31dcbddf25e, 0x5a6dd8bae52be772) },
    { 0, -62, TRIG_U128(0xf2a15d1fd60500d6, 0x9438ba5d2031b4d0) },
    { 1, -70, TRIG_U128(0x8671cb1f833c0957, 0x987226ca65cb7c1a) },
    { 0, -80, TRIG_U128(0xf9666cf2db6b1cb7, 0x38dd68ce5a1a05c4) },
    { 1, -89, TRIG_U128(0xc355ba2967b5ca73, 0x2d6ab7f31ee13ec5) },
};

static inline dint_t neg_dint_(dint_t v)
{
    v.sgn = !v.sgn;
    return v;
}

/* Bits [start, start+63] of the zero-extended 6-limb little-endian p, for any
 * signed start (start>>6 / start&63 give the Euclidean quotient/remainder via
 * arithmetic right shift). */
static inline uint64_t trig_phd_word_(const uint64_t p[6], int64_t start)
{
    int64_t idx = start >> 6;
    int off = (int)(start & 63);
    uint64_t lo = (idx >= 0 && idx < 6) ? p[idx] : 0;
    if (!off)
        return lo;
    uint64_t hi = (idx + 1 >= 0 && idx + 1 < 6) ? p[idx + 1] : 0;
    return (lo >> off) | (hi << (64 - off));
}

/* Payne-Hanek reduction to a 128-bit Dint reduced angle, for any finite
 * x >= 2^-27.  Returns (quadrant, r in [-pi/4, pi/4]). */
static inline int64_t payne_hanek_dint_(double x, dint_t *out)
{
    uint64_t bits = reinterpret(uint64_t, x);
    int64_t e = (int64_t)((bits >> 52) & 0x7ff);
    unsigned __int128 sig = (unsigned __int128)((bits & 0x000FFFFFFFFFFFFF) | 0x0010000000000000);

    int64_t m = e - 1075;
    int64_t w0 = ((m - 2) > 0 ? (m - 2) : 0) / 64;
#define TRIG_WD(i) ((uint64_t)(w0 + (i)) < 24 ? (unsigned __int128)TRIG_FRAC_2_PI[w0 + (i)] : 0)
    unsigned __int128 wa = TRIG_WD(0), wb = TRIG_WD(1), wc = TRIG_WD(2), wd = TRIG_WD(3), we = TRIG_WD(4);
#undef TRIG_WD
    unsigned __int128 pa = sig * wa, pb = sig * wb, pc = sig * wc, pd = sig * wd, pe = sig * we;
    const unsigned __int128 MASK = 0xFFFFFFFFFFFFFFFFu;

    uint64_t p0 = (uint64_t)pe;
    unsigned __int128 c = (pe >> 64) + (pd & MASK);
    uint64_t p1 = (uint64_t)c;
    c = (c >> 64) + (pd >> 64) + (pc & MASK);
    uint64_t p2 = (uint64_t)c;
    c = (c >> 64) + (pc >> 64) + (pb & MASK);
    uint64_t p3 = (uint64_t)c;
    c = (c >> 64) + (pb >> 64) + (pa & MASK);
    uint64_t p4 = (uint64_t)c;
    uint64_t p5 = (uint64_t)((c >> 64) + (pa >> 64));
    uint64_t p[6] = { p0, p1, p2, p3, p4, p5 };

    int64_t b = 320 + 64 * w0 - m;
    unsigned __int128 hi = ((unsigned __int128)trig_phd_word_(p, b - 64) << 64) | trig_phd_word_(p, b - 128);
    unsigned __int128 lo = ((unsigned __int128)trig_phd_word_(p, b - 192) << 64) | trig_phd_word_(p, b - 256);

    _Bool sgn = (hi >> 127) == 1;
    int64_t quadrant = (int64_t)trig_phd_word_(p, b) + (sgn ? 1 : 0);

    unsigned __int128 mag_hi, mag_lo;
    if (sgn) {
        unsigned __int128 nlo = ~lo;
        _Bool carry = (nlo == ~(unsigned __int128)0);
        mag_lo = nlo + 1;
        mag_hi = ~hi + (carry ? 1 : 0);
    } else {
        mag_hi = hi;
        mag_lo = lo;
    }

    int lz = (mag_hi != 0) ? dint_clz_(mag_hi) : 128 + dint_clz_(mag_lo);
    unsigned __int128 m128;
    if (lz == 0)
        m128 = mag_hi;
    else if (lz < 128)
        m128 = (mag_hi << lz) | (mag_lo >> (128 - lz));
    else
        m128 = mag_lo << (lz - 128);

    dint_t frac = { sgn, -1 - lz, m128 };
    *out = dint_mul_(&frac, &PIO2_DINT);
    return quadrant;
}

/* Horner of a Dint polynomial in u, low-degree first. */
static inline dint_t poly_dint_(const dint_t *u, const dint_t *coeffs, int n)
{
    dint_t acc = coeffs[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        dint_t t = dint_mul_(u, &acc);
        acc = dint_add_(&coeffs[i], &t);
    }
    return acc;
}

static inline dint_t sin_dint_(const dint_t *r)
{
    dint_t u = dint_mul_(r, r);
    dint_t p = poly_dint_(&u, SIN_DINT, 14);
    return dint_mul_(r, &p);
}

static inline dint_t cos_dint_(const dint_t *r)
{
    dint_t u = dint_mul_(r, r);
    return poly_dint_(&u, COS_DINT, 14);
}

static inline void sin_cos_dint_(const dint_t *r, dint_t *s, dint_t *cc)
{
    dint_t u = dint_mul_(r, r);
    dint_t ps = poly_dint_(&u, SIN_DINT, 14);
    *s = dint_mul_(r, &ps);
    *cc = poly_dint_(&u, COS_DINT, 14);
}

/* 1/c as a Dint, two Newton steps y*(2 - c*y) from the f64 seed 1/c. */
static inline dint_t recip_dint_(const dint_t *c)
{
    dint_t y = dint_from_f64_(1.0 / dint_to_f64_(c));
    for (int i = 0; i < 2; ++i) {
        dint_t cy = dint_mul_(c, &y);
        dint_t ncy = neg_dint_(cy);
        dint_t t = dint_add_(&TWO_DINT, &ncy);
        y = dint_mul_(&y, &t);
    }
    return y;
}

static inline dint_t select_sin_dint_(int64_t q, dint_t s, dint_t c)
{
    dint_t v = (q & 1) ? c : s;
    return (q & 2) ? neg_dint_(v) : v;
}

static inline dint_t select_cos_dint_(int64_t q, dint_t s, dint_t c)
{
    dint_t v = (q & 1) ? s : c;
    return ((q + 1) & 2) ? neg_dint_(v) : v;
}

#endif

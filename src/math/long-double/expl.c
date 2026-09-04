/* Correctly rounded binary128 exponential family.
 * Ported from metallic-rs 76a9d7f0bce0e2a1dd271b106801325beb866dc3. */
#include "kernel/binary128.h"
#include "kernel/uint.h"
#include "kernel/exptab.h"
#include "kernel/exp_engine.h"

#include <stdbool.h>
#include <stdint.h>

#define ZIV_GATE F128_EXP_ZIV_GATE
#define GUARD F128_EXP_GUARD
#define GUARD_MASK F128_EXP_GUARD_MASK
#define GUARD_HALF F128_EXP_GUARD_HALF
#define SATURATE (((u128)(F128_BIAS + 15)) << F128_EXP_SHIFT)
#define TINY (((u128)(F128_BIAS - 120)) << F128_EXP_SHIFT)
#define MINUS_ONE ((((u128)(F128_BIAS + 6)) << F128_EXP_SHIFT) | ((u128)1 << (F128_EXP_SHIFT - 2)))
#define TINY_EXPM1 (((u128)(F128_BIAS - 114)) << F128_EXP_SHIFT)
#define POLY_LIMIT ((u128)1 << 110)
#define EXPM1_FRAME_EXP (-6)
#define MID_EXP (-19)
#define MAX_SHIFT 7

typedef f128_exp_fast_result_t exp_fast_result;
typedef struct { int n; u128 high, low; } expm1_result;
typedef f128_exp_wide_result_t exp_wide_result;
typedef struct { u128 lo, hi; } exp_product;
typedef struct { u128 mantissa; int exponent; } exp_parts;

static long double exp_accurate(u128, int, bool, const exp_reduction *)
    __attribute__((cold, noinline));
static exp_wide_result exp_wide(u128, int, bool, const exp_reduction *);
static long double expm1_accurate(u128, int, bool)
    __attribute__((cold, noinline));
static long double expm1_small(u128, int, u384_t, bool)
    __attribute__((cold, noinline));

static exp_product wide_product(u128 a, u128 b)
{
    exp_product p;
    wmul_(a, b, &p.hi, &p.lo);
    return p;
}

static exp_parts split_magnitude(u128 bits)
{
    exp_parts p;
    f128_split_(bits, &p.mantissa, &p.exponent);
    return p;
}

static unsigned u128_tz(u128 x)
{
    uint64_t low = (uint64_t)x;
    return low ? __builtin_ctzll(low) : 64 + __builtin_ctzll((uint64_t)(x >> 64));
}

static unsigned u128_lz(u128 x)
{
    uint64_t high = (uint64_t)(x >> 64);
    uint64_t low = (uint64_t)x;
    return high ? __builtin_clzll(high) : low ? 64 + __builtin_clzll(low) : 128;
}

static u128 abs_diff(u128 a, u128 b)
{
    return a > b ? a - b : b - a;
}

static u384_t reduce(u128 m, int e, const u128 head[2])
{
    exp_product a = wide_product(m, head[0]);
    exp_product b = wide_product(m, head[1]);
    u128 middle = a.hi + b.lo;
    bool carry = middle < a.hi;
    u384_t product = {{a.lo, middle, b.hi + carry}};
    unsigned shift = 108 - e;
    u128 low, mid, high;

    if (shift < 128) {
        low = product.limb[0]; mid = product.limb[1]; high = product.limb[2];
    } else {
        low = product.limb[1]; mid = product.limb[2]; high = 0;
    }
    unsigned bits = shift & 127;
    return (u384_t){{
        (low >> bits) | (mid << 1 << (127 - bits)),
        (mid >> bits) | (high << 1 << (127 - bits)),
        high >> bits,
    }};
}

static uint64_t frame_funnel(uint64_t low, uint64_t high, unsigned shift)
{
    return (low >> shift) | (high << 1 << (63 - shift));
}

static unsigned reduction_tz(const exp_reduction *l)
{
    return l->head[0] ? u128_tz(l->head[0]) : 128 + u128_tz(l->head[1]);
}

static inline __attribute__((always_inline)) exp_fast_result exp_frame(u128 m, int e, const exp_reduction *l, bool negative)
{
    /* Bits [192, 384) of m * head suffice: the fraction window bottom sits at
     * bit 222 or above (shift <= 228 below), so the five dropped partial
     * products contribute < 2^195 — a one-sided deficit 27+ bits under the
     * window that at worst leaves the fraction one unit short. */
    uint64_t m0 = (uint64_t)m;
    uint64_t m1 = (uint64_t)(m >> 64);
    uint64_t h1 = (uint64_t)(l->head[0] >> 64);
    uint64_t h2 = (uint64_t)l->head[1];
    uint64_t h3 = (uint64_t)(l->head[1] >> 64);
    u128 a = umulditi3_(m0, h3);
    u128 b = umulditi3_(m1, h2)
        + ((umulditi3_(m0, h2) >> 64) + (umulditi3_(m1, h1) >> 64));
    u128 middle = a + b;
    u128 top = umulditi3_(m1, h3) + (middle >> 64)
        + ((u128)(middle < a) << 64);
    unsigned shift = 108 - e;
    unsigned bits = shift & 63;
    uint64_t low = (uint64_t)middle;
    uint64_t high = (uint64_t)top;
    uint64_t integer = (uint64_t)(top >> 64);

    if (shift >= 128) { low = high; high = integer; integer = 0; }
    if (shift >= 192) { low = high; high = integer; integer = 0; }

    u128 fraction = (u128)frame_funnel(low, high, bits)
                  | (u128)frame_funnel(high, integer, bits) << 64;
    u128 mask = negative ? ~(u128)0 : 0;
    bool borrow = negative && u128_tz(m) + reduction_tz(l) >= shift + 128;
    u128 before = fraction ^ mask;
    fraction = before + borrow;
    bool carry = fraction < before;
    int n = ((int)(integer >> bits) ^ (int)mask) + carry;
    return (exp_fast_result){n, fraction};
}

static u128 correction(u128 m, int e, u128 tail)
{
    int shift = 236 - e;
    return shift >= 256 ? 0 : mhi_(m, tail) >> (shift - 128);
}

static void exp_index(u128 f, unsigned *i0, unsigned *i1, unsigned *i2, u128 *t)
{
    *i0 = f >> 122;
    *i1 = (f >> 116) & 63;
    *i2 = (f >> 110) & 63;
    *t = f & (((u128)1 << 110) - 1);
}

/* Q1.127 product, up to 5 units short of exact: mhi_approx_ loses <= 2
 * pre-shift units and the doubling drops the bit below the point. */
static u128 mul127_approx(u128 a, u128 b)
{
    return mhi_approx_(a, b) << 1;
}

/* floor(a * b / 2^64) for a 64-bit a, at most 1 unit short. */
static u128 window_mul(uint64_t a, u128 b)
{
    return umulditi3_(a, (uint64_t)(b >> 64)) + (umulditi3_(a, (uint64_t)b) >> 64);
}

/* EXP_COEF rescaled for a 2^18-normalized argument: EXP_G[k] is the Q0.128
 * truncation of EXP_COEF[k] >> 18k, so the Horner chain multiplies by
 * sigma = t << 18 instead of t and the low-order tail fits the 64-bit
 * window below 2^-64 (EXP_G5W/EXP_G6W are EXP_COEF[4..5] >> 18(4..5)). */
static const u128 EXP_G[4] = {
    U128(0xb17217f7d1cf79ab, 0xc9e3b39803f2f6af),
    U128(0x00000f5fdeffc162, 0xc754378b583764b9),
    U128(0x0000000000e35846, 0xb82505fc599d3b15),
    U128(0x0000000000000009, 0xd955b7dd273b94e6),
};
#define EXP_G5W UINT64_C(0x00005761ff9e299c)
#define EXP_G6W UINT64_C(0x0000000002861225)

static u256_t mul255(u256_t a, u256_t b)
{
    u256_t p = u256_mul_hi_(a, b);
    return (u256_t){{p.limb[0] << 1, (p.limb[1] << 1) | (p.limb[0] >> 127)}};
}

/* One-sided error budget, in units of 2^-128 on the returned significand
 * (everything below is a deficit, never an excess).  The polynomial runs on
 * sigma = t << 18 (exact); its truncations reach p only through the >> 19,
 * so p is at most 2 units short.  Degrees 1-2 keep the full 128-bit sigma:
 * with only the top 64 bits their dropped cross terms would be 2^25 and 2^5
 * units.  Degree 3 drops sigma_low * h4 < 2^-124 pre-shift; degrees 4+ live
 * in the 64-bit window with 2^-128-scale truncation.  Each mul127_approx is
 * <= 5 short, the final high half <= 5, amplified by the remaining <= 2x
 * factors: tables + polynomial stay under ~40 total, within the Ziv gate. */
static inline __attribute__((always_inline)) exp_fast_result exp_fast(int n, u128 f)
{
    unsigned i0, i1, i2;
    u128 t;
    exp_index(f, &i0, &i1, &i2, &t);
    u128 s = t << 18;
    uint64_t z = (uint64_t)(s >> 64);
    uint64_t h5 = EXP_G5W + mul_hi_64_(z, EXP_G6W);
    u128 h4 = EXP_G[3] + mul_hi_64_(z, h5);
    u128 h3 = EXP_G[2] + window_mul(z, h4);
    u128 h2 = EXP_G[1] + mhi_approx_(s, h3);
    u128 h1 = EXP_G[0] + mhi_approx_(s, h2);
    u128 p = ((u128)1 << 127) + (mhi_approx_(s, h1) >> 19);
    u128 r = mul127_approx(mul127_approx(T1[i1][1], T2[i2][1]), p);
    u128 hi = mhi_approx_(T0[i0][1], r);
    u128 carry = hi >> 127;
    return (exp_fast_result){n + (int)carry, carry ? hi : hi << 1};
}

f128_exp_fast_result_t __metallic_f128_exp_fast(int n, u128 f)
{
    return exp_fast(n, f);
}

f128_exp_wide_result_t __metallic_f128_exp2_frame(u384_t y)
{
    unsigned i0, i1, i2;
    u128 tail;
    exp_index(y.limb[1], &i0, &i1, &i2, &tail);
    u256_t t = {{y.limb[0], tail}};
    u256_t q = {{EXP_COEF[12][0], EXP_COEF[12][1]}};
    for (int i = 11; i >= 0; --i)
        q = u256_add_((u256_t){{EXP_COEF[i][0], EXP_COEF[i][1]}}, u256_mul_hi_(t, q));
    u256_t p = u256_mul_hi_(t, q);
    p = (u256_t){{(p.limb[0] >> 1) | (p.limb[1] << 127), (p.limb[1] >> 1) | (u128)1 << 127}};
    u256_t a = {{T1[i1][0], T1[i1][1]}};
    u256_t b = {{T2[i2][0], T2[i2][1]}};
    u256_t r = mul255(mul255(a, b), p);
    a = (u256_t){{T0[i0][0], T0[i0][1]}};
    r = u256_mul_hi_(a, r);
    unsigned carry = r.limb[1] >> 127;
    if (!carry) r = (u256_t){{r.limb[0] << 1, (r.limb[1] << 1) | (r.limb[0] >> 127)}};
    return (exp_wide_result){(int)y.limb[2] + (int)carry, r};
}

static exp_wide_result exp_wide(u128 m, int e, bool negative, const exp_reduction *l)
{
    u384_t y = reduce(m, e, l->head);
    y = u384_add_(y, (u384_t){{correction(m, e, l->tail), 0, 0}});
    if (negative) y = u384_neg_(y);
    return __metallic_f128_exp2_frame(y);
}

static unsigned discarded(int n)
{
    int subnormal = -16382 - n;
    return 15 + (subnormal > 0 ? subnormal : 0);
}

bool __metallic_f128_exp_undecided(int n, u128 r, u128 gate)
{
    unsigned shift = discarded(n);
    if (n > 16383 || shift > 128) return false;
    u128 mask = ~(u128)0 >> (128 - shift);
    return abs_diff(r & mask, (u128)1 << (shift - 1)) <= gate;
}

long double __metallic_f128_exp_round(int n, u128 high, u128 low)
{
    if (n > 16383) return f128_from_bits_(F128_EXP_MASK);
    unsigned shift = discarded(n);
    if (shift > 128) return 0.0L;
    u128 rest = high & (~(u128)0 >> (128 - shift));
    u128 half = (u128)1 << (shift - 1);
    u128 mantissa = shift == 128 ? 0 : high >> shift;
    bool up = rest > half || (rest == half && (low != 0 || (mantissa & 1)));
    u128 packed = n >= -16382
        ? ((u128)(n + F128_BIAS) << F128_EXP_SHIFT) + (mantissa - F128_IMPLICIT_BIT)
        : mantissa;
    return f128_from_bits_(packed + up);
}

static long double exp_accurate(u128 m, int e, bool negative, const exp_reduction *l)
{
    exp_wide_result w = exp_wide(m, e, negative, l);
    return __metallic_f128_exp_round(w.n, w.r.limb[1], w.r.limb[0]);
}

static inline __attribute__((always_inline)) long double
exp_finish(exp_parts s, exp_fast_result f, bool negative, const exp_reduction *l)
{
    f = exp_fast(f.n, f.r);
    if ((unsigned)(f.n + 16382) > 32765) {
        if (__metallic_f128_exp_undecided(f.n, f.r, ZIV_GATE))
            return exp_accurate(s.mantissa, s.exponent, negative, l);
        return __metallic_f128_exp_round(f.n, f.r, 0);
    }
    u128 rest = f.r & GUARD_MASK;
    if (abs_diff(rest, GUARD_HALF) <= ZIV_GATE)
        return exp_accurate(s.mantissa, s.exponent, negative, l);
    return f128_from_bits_(((u128)(f.n + F128_BIAS) << F128_EXP_SHIFT)
        + ((f.r >> GUARD) - F128_IMPLICIT_BIT) + (rest > GUARD_HALF));
}

static long double exp_generic(long double x, const exp_reduction *l)
{
    u128 bits = f128_bits_(x);
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (magnitude >= SATURATE) {
        if (magnitude > F128_EXP_MASK) return f128_from_bits_(bits | F128_QUIET_BIT);
        return bits & F128_SIGN_MASK ? 0.0L : f128_from_bits_(F128_EXP_MASK);
    }
    if (magnitude < TINY) return 1.0L;
    exp_parts s = split_magnitude(magnitude);
    bool negative = (bits & F128_SIGN_MASK) != 0;
    exp_fast_result f = exp_frame(s.mantissa, s.exponent, l, negative);
    return exp_finish(s, f, negative, l);
}

/* exp2's reduction is an exact funnel shift: x = m·2^(e-112), so the frame is
 * the mantissa split at the binary point — no multiplication by a constant.
 * The TINY exit bounds e to [-120, 14], keeping every shift count in range. */
static exp_fast_result exp2_frame(u128 m, int e, bool negative)
{
    int left = e + 16;
    u128 fraction;
    int n;
    bool borrow;
    if (left >= 0) {
        fraction = m << left;
        n = left ? (int)(m >> (112 - e)) : 0;
        borrow = negative;
    } else {
        fraction = m >> -left;
        n = 0;
        borrow = negative && u128_tz(m) >= (unsigned)-left;
    }
    u128 mask = negative ? ~(u128)0 : 0;
    u128 before = fraction ^ mask;
    fraction = before + borrow;
    bool carry = fraction < before;
    return (exp_fast_result){((n ^ (int)mask) + carry), fraction};
}

long double expl(long double x) { return exp_generic(x, &LOG2E); }
long double exp10l(long double x) { return exp_generic(x, &LOG2_10); }

long double exp2l(long double x)
{
    u128 bits = f128_bits_(x);
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (magnitude >= SATURATE) {
        if (magnitude > F128_EXP_MASK) return f128_from_bits_(bits | F128_QUIET_BIT);
        return bits & F128_SIGN_MASK ? 0.0L : f128_from_bits_(F128_EXP_MASK);
    }
    if (magnitude < TINY) return 1.0L;
    exp_parts s = split_magnitude(magnitude);
    bool negative = (bits & F128_SIGN_MASK) != 0;
    exp_fast_result f = exp2_frame(s.mantissa, s.exponent, negative);
    return exp_finish(s, f, negative, &ONE);
}

static uint64_t inv_factorial64(unsigned j) { return INV_FACT[j] >> 64; }

static expm1_result apply_correction(u128 m15, u128 d, int e, bool negative)
{
    exp_product p = wide_product(m15, d);
    u128 mask = negative ? ~(u128)0 : 0;
    u128 low = (p.lo ^ mask) - mask;
    u128 add = (p.hi ^ mask) + (negative && p.lo == 0);
    u128 high = m15 + add;
    bool carried = high < m15;
    if (carried != negative)
        return (expm1_result){e + 1, ((u128)1 << 127) | high >> 1, high << 127 | low >> 1};
    if (high >> 127) return (expm1_result){e, high, low};
    return (expm1_result){e - 1, high << 1 | low >> 127, low << 1};
}

static expm1_result expm1_mid(u128 m15, int e, bool negative)
{
    u128 t = m15 >> (-1 - e);
    u128 w = mhi_(t, t);
    uint64_t short_w = w >> 64;
    uint64_t tail = inv_factorial64(12) + mul_hi_64_(short_w, inv_factorial64(14));
    u128 even = INV_FACT[10] + mhi_(w, (u128)tail << 64);
    static const unsigned ei[] = {8, 6, 4, 2, 0};
    for (unsigned i = 0; i < 5; ++i) even = INV_FACT[ei[i]] + mhi_(w, even);
    tail = inv_factorial64(11) + mul_hi_64_(short_w, inv_factorial64(13));
    u128 odd = INV_FACT[9] + mhi_(w, (u128)tail << 64);
    static const unsigned oi[] = {7, 5, 3, 1};
    for (unsigned i = 0; i < 4; ++i) odd = INV_FACT[oi[i]] + mhi_(w, odd);
    u128 mask = negative ? ~(u128)0 : 0;
    u128 h = even + ((mhi_(t, odd) ^ mask) - mask);
    return apply_correction(m15, mhi_(t, h), e, negative);
}

static expm1_result expm1_tiny(u128 m15, int e, bool negative)
{
    u128 t = m15 >> (-1 - e);
    u128 mask = negative ? ~(u128)0 : 0;
    uint64_t narrow = (uint64_t)mask;
    uint64_t short_t = t >> 64;
    uint64_t term64 = mul_hi_64_(short_t, inv_factorial64(5)) ^ narrow;
    uint64_t tail = inv_factorial64(4) + (term64 - narrow);
    u128 term = mhi_(t, (u128)tail << 64) ^ mask;
    u128 h = INV_FACT[3] + (term - mask);
    for (int j = 2; j >= 0; --j) {
        term = mhi_(t, h) ^ mask;
        h = INV_FACT[j] + (term - mask);
    }
    return apply_correction(m15, mhi_(t, h), e, negative);
}

static bool subtract_one(exp_fast_result f, expm1_result *out)
{
    u128 v;
    int frame;
    if (f.n >= 0) {
        v = f.r - (f.n < 128 ? (u128)1 << (127 - f.n) : 0);
        frame = f.n;
    } else {
        v = -(f.r >> (-1 - f.n));
        frame = -1;
    }
    unsigned shift = u128_lz(v);
    if (shift > MAX_SHIFT) return false;
    *out = (expm1_result){frame - (int)shift, v << shift, ZIV_GATE << shift};
    return true;
}

static long double expm1_small(u128 m, int e, u384_t y, bool negative)
{
    u256_t t = {{y.limb[0], y.limb[1]}};
    u256_t q = {{EXP_COEF[12][0], EXP_COEF[12][1]}};
    for (int i = 11; i >= 0; --i) {
        u256_t term = u256_mul_hi_(t, q);
        u256_t c = {{EXP_COEF[i][0], EXP_COEF[i][1]}};
        q = negative ? u256_sub_(c, term) : u256_add_(c, term);
    }
    u256_t log2e = {{LOG2E.head[0], LOG2E.head[1]}};
    u256_t g = u256_mul_hi_(q, log2e);
    exp_product a = wide_product(m, g.limb[0]);
    exp_product b = wide_product(m, g.limb[1]);
    u128 middle = a.hi + b.lo;
    bool carry = middle < a.hi;
    u384_t product = {{a.lo, middle, b.hi + carry}};
    unsigned shift = u384_clz_(product);
    u384_t p = u384_shl_(product, shift);
    return __metallic_f128_exp_round(e + 19 - (int)shift, p.limb[2],
        p.limb[1] | (p.limb[0] != 0));
}

static long double expm1_accurate(u128 m, int e, bool negative)
{
    exp_wide_result w = exp_wide(m, e, negative, &LOG2E);
    u256_t v;
    int frame;
    if (w.n >= 0) {
        u256_t one = w.n >= 256 ? (u256_t){{1, 0}}
                 : w.n >= 128 ? (u256_t){{(u128)1 << (255 - w.n), 0}}
                 : (u256_t){{0, (u128)1 << (127 - w.n)}};
        v = u256_sub_(w.r, one);
        frame = w.n;
    } else {
        unsigned k = -1 - w.n;
        u256_t shifted = {{(w.r.limb[0] >> k) | (w.r.limb[1] << 1 << (127 - k)), w.r.limb[1] >> k}};
        v = u256_sub_((u256_t){{0, 0}}, shifted);
        frame = -1;
    }
    unsigned shift = u128_lz(v.limb[1]);
    u128 high = (v.limb[1] << shift) | (v.limb[0] >> 1 >> (127 - shift));
    return __metallic_f128_exp_round(frame - (int)shift, high,
        v.limb[0] << shift);
}

long double expm1l(long double x)
{
    u128 bits = f128_bits_(x);
    u128 magnitude = bits & ~F128_SIGN_MASK;
    bool negative = (bits & F128_SIGN_MASK) != 0;
    if (magnitude >= SATURATE) {
        if (magnitude > F128_EXP_MASK) return f128_from_bits_(bits | F128_QUIET_BIT);
        return negative ? -1.0L : f128_from_bits_(F128_EXP_MASK);
    }
    if (negative && magnitude >= MINUS_ONE) return -1.0L;
    if (magnitude < TINY_EXPM1) return x;
    exp_parts s = split_magnitude(magnitude);
    long double value;
    if (s.exponent >= EXPM1_FRAME_EXP) {
        exp_fast_result f = exp_frame(s.mantissa, s.exponent, &LOG2E, negative);
        expm1_result d;
        f = exp_fast(f.n, f.r);
        if (subtract_one(f, &d)
            && !__metallic_f128_exp_undecided(d.n, d.high, d.low))
            value = __metallic_f128_exp_round(d.n, d.high, 0);
        else
            value = expm1_accurate(s.mantissa, s.exponent, negative);
    } else {
        expm1_result r = s.exponent >= MID_EXP
            ? expm1_mid(s.mantissa << 15, s.exponent, negative)
            : expm1_tiny(s.mantissa << 15, s.exponent, negative);
        if (__metallic_f128_exp_undecided(r.n, r.high, ZIV_GATE)) {
            u384_t y = reduce(s.mantissa, s.exponent, LOG2E.head);
            value = y.limb[2] == 0 && y.limb[1] < POLY_LIMIT
                ? expm1_small(s.mantissa, s.exponent, y, negative)
                : expm1_accurate(s.mantissa, s.exponent, negative);
        } else {
            value = __metallic_f128_exp_round(r.n, r.high, r.low);
        }
    }
    return f128_from_bits_(f128_bits_(value) | (negative ? F128_SIGN_MASK : 0));
}

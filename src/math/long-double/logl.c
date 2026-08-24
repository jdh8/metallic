/* Correctly rounded binary128 natural logarithm.
 * Ported from metallic-rs 76a9d7f0bce0e2a1dd271b106801325beb866dc3. */
#include "kernel/binary128.h"
#include "kernel/uint.h"
#include "kernel/logtab.h"

#include <stdbool.h>
#include <stdint.h>

typedef __int128 i128;

#define LOG_ZIV_GATE ((u128)1 << 75)
#define LOG_FAST_FLOOR ((u128)1 << 70)
#define LOG_FRAME_EXP (F128_BIAS + 41)

static long double log_accurate(int, unsigned, u256_t);

static u256_t negate_if(u256_t s, bool negative)
{
    u128 mask = 0 - (u128)negative;
    u128 low = (s.limb[0] ^ mask) + (mask & 1);
    bool carry = low < (s.limb[0] ^ mask);
    return U256(low, (s.limb[1] ^ mask) + carry);
}

static long double log_edge(u128 bits)
{
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (magnitude > F128_EXP_MASK)
        return f128_from_bits_(bits | F128_QUIET_BIT);
    if (magnitude == 0)
        return f128_from_bits_(F128_SIGN_MASK | F128_EXP_MASK);
    if (bits & F128_SIGN_MASK)
        return f128_from_bits_(F128_EXP_MASK | F128_QUIET_BIT);
    return f128_from_bits_(F128_EXP_MASK);
}

static unsigned crude_log2(u128 m)
{
    uint64_t h = m >> 49;
    uint64_t entry = LOG_CRUDE[(h >> 55) & 255];
    uint64_t fit = (entry >> 23) + mul_hi_64_(h << 9, entry & (((uint64_t)1 << 23) - 1));
    return (fit + ((uint64_t)1 << 11)) >> 12;
}

static void log_index(unsigned j, unsigned *j0, unsigned *j1, unsigned *j2)
{
    *j0 = j >> 12;
    *j1 = (j >> 6) & 63;
    *j2 = j & 63;
}

static u128 reciprocal(unsigned j)
{
    unsigned j0, j1, j2;
    log_index(j, &j0, &j1, &j2);
    return (u128)((uint64_t)RECIP0[j0] * RECIP1[j1]) * RECIP2[j2];
}

static int64_t log_coefficient(unsigned k)
{
    return LOG_COEF[k][1] >> 64;
}

static int64_t mul_hi_i64(int64_t x, int64_t y)
{
    return (i128)x * y >> 64;
}

static i128 mul_hi_i128(i128 x, u128 y)
{
    u128 sign = (u128)(x >> 127);
    return (i128)(mhi_approx_((u128)x, y) - (sign & y));
}

static u128 sqr_hi(i128 x)
{
    u128 ux = (u128)x;
    return mhi_approx_(ux, ux) - ((((u128)(x >> 127)) & ux) << 1);
}

static i128 log1p_fast(i128 z)
{
    i128 narrow = z >> 17;
    int64_t short_z = z >> 81;
    int64_t tail = log_coefficient(6);
    for (int k = 5; k >= 4; --k)
        tail = log_coefficient(k) - mul_hi_i64(short_z, tail);

    u128 a = LOG_COEF[0][1] - (u128)mul_hi_i128(narrow, LOG_COEF[1][1]);
    u128 b = LOG_COEF[2][1] - (u128)mul_hi_i128(narrow, LOG_COEF[3][1]);
    u128 nn = sqr_hi(narrow);
    u128 n4 = mhi_approx_(nn, nn);
    u128 q = a + mhi_approx_(nn, b) + (u128)mul_hi_64_((uint64_t)n4, (uint64_t)tail);
    return (i128)((u128)mul_hi_i128(z, q) << 1);
}

static u256_t log_scale(int e, const u128 l[2])
{
    u128 a = e < 0 ? (unsigned)-e : (unsigned)e;
    u128 high, low;
    wmul_(a, l[0], &high, &low);
    return negate_if(U256(low, high + a * l[1]), e < 0);
}

static u256_t log_fast(int e, unsigned j, i128 z)
{
    unsigned j0, j1, j2;
    log_index(j, &j0, &j1, &j2);
    i128 l = log1p_fast(z);
    const u128 ln2[2] = {LOG_LN2[1], LOG_LN2[2]};
    u256_t s = log_scale(e, ln2);
    s = u256_add_(s, U256(LOG0[j0][1], LOG0[j0][2]));
    s = u256_add_(s, U256(LOG1[j1][1], LOG1[j1][2]));
    s = u256_add_(s, U256(LOG2[j2][1], LOG2[j2][2]));
    return u256_add_(s, U256((u128)l << 69, (u128)(l >> 59)));
}

static u384_t log_scale_wide(int e, const u128 l[3])
{
    u128 a = e < 0 ? (unsigned)-e : (unsigned)e;
    u128 high, low, top, middle;
    wmul_(a, l[0], &high, &low);
    wmul_(a, l[1], &top, &middle);
    u128 sum = middle + high;
    bool carry = sum < middle;
    u384_t p = U384(low, sum, top + carry + a * l[2]);
    return e < 0 ? u384_neg_(p) : p;
}

static u256_t signed_product(u256_t x, u256_t y, bool negative)
{
    u256_t high = u256_mul_hi_(x, y);
    return negative ? u256_sub_(high, y) : high;
}

static u256_t signed_shift_right(u256_t x, unsigned shift)
{
    u128 high = (u128)((i128)x.limb[1] >> shift);
    return U256((x.limb[0] >> shift) | (x.limb[1] << 1 << (127 - shift)), high);
}

static u384_t log1p_wide(u256_t d)
{
    bool negative = d.limb[1] >> 127;
    u256_t z = u256_shl_(d, 68);
    u256_t q = U256(LOG_COEF[13][0], LOG_COEF[13][1]);
    for (int i = 12; i >= 0; --i) {
        u256_t c = U256(LOG_COEF[i][0], LOG_COEF[i][1]);
        q = u256_sub_(c, signed_shift_right(signed_product(z, q, negative), 17));
    }
    u256_t magnitude = negative ? u256_sub_(U256(0, 0), d) : d;
    if (magnitude.limb[0] == 0 && magnitude.limb[1] == 0)
        return U384(0, 0, 0);
    unsigned leading = u256_clz_(magnitude);
    u256_t w = u256_mul_hi_(u256_shl_(magnitude, leading), q);
    u384_t frame;
    if (leading <= 138) {
        frame = u384_shl_(U384(w.limb[0], w.limb[1], 0), 138 - leading);
    } else {
        unsigned k = leading - 138;
        frame = U384((w.limb[0] >> k) | (w.limb[1] << 1 << (127 - k)),
                     w.limb[1] >> k, 0);
    }
    return negative ? u384_neg_(frame) : frame;
}

static long double log_round(u384_t s)
{
    bool negative = s.limb[2] >> 127;
    u384_t magnitude = negative ? u384_neg_(s) : s;
    if (!(magnitude.limb[0] | magnitude.limb[1] | magnitude.limb[2]))
        return 0.0L;
    unsigned leading = u384_clz_(magnitude);
    unsigned shift = 271 - leading;
    u128 mantissa = u384_extract_u128_(magnitude, shift);
    bool round_bit = (magnitude.limb[(shift - 1) / 128] >> ((shift - 1) % 128)) & 1;
    bool up = round_bit && (u384_any_below_(magnitude, shift - 1) || (mantissa & 1));
    return f128_from_bits_((negative ? F128_SIGN_MASK : 0)
        | ((u128)(LOG_FRAME_EXP - leading) << F128_EXP_SHIFT)
        + (mantissa - F128_IMPLICIT_BIT) + up);
}

static long double log_accurate(int e, unsigned j, u256_t d)
{
    unsigned j0, j1, j2;
    log_index(j, &j0, &j1, &j2);
    u384_t s = log_scale_wide(e, LOG_LN2);
    s = u384_add_(s, U384(LOG0[j0][0], LOG0[j0][1], LOG0[j0][2]));
    s = u384_add_(s, U384(LOG1[j1][0], LOG1[j1][1], LOG1[j1][2]));
    s = u384_add_(s, U384(LOG2[j2][0], LOG2[j2][1], LOG2[j2][2]));
    return log_round(u384_add_(s, log1p_wide(d)));
}

long double logl(long double x)
{
    u128 bits = f128_bits_(x);
    if (bits - 1 >= F128_EXP_MASK - 1)
        return log_edge(bits);

    u128 m;
    int e;
    f128_split_(bits, &m, &e);
    unsigned j = crude_log2(m);
    u128 high, low;
    wmul_(m, reciprocal(j), &high, &low);
    u256_t d = U256(low, high - ((u128)1 << 77));
    i128 z = (i128)((d.limb[1] << 68) | (low >> 60));
    u256_t s = log_fast(e, j, z);
    bool negative = s.limb[1] >> 127;
    u256_t magnitude = negate_if(s, negative);
    if (magnitude.limb[1] < LOG_FAST_FLOOR)
        return log_accurate(e, j, d);

    unsigned leading = __builtin_clzll((uint64_t)(magnitude.limb[1] >> 64));
    unsigned shift = 79 - leading;
    unsigned window = leading - 15;
    u128 v = (u128)funnel_((uint64_t)magnitude.limb[0],
                           (uint64_t)(magnitude.limb[0] >> 64), window) << 64
           | (u128)((uint64_t)magnitude.limb[0] << window);
    u128 bound = (u128)((uint64_t)(LOG_ZIV_GATE >> 64) << window) << 64;
    if (((v ^ ((u128)1 << 127)) + bound) <= bound << 1)
        return log_accurate(e, j, d);

    u128 mantissa = (u128)funnel_down_((uint64_t)magnitude.limb[1],
                                       (uint64_t)(magnitude.limb[1] >> 64), shift) << 64
                  | funnel_down_((uint64_t)(magnitude.limb[0] >> 64),
                                 (uint64_t)magnitude.limb[1], shift);
    return f128_from_bits_((negative ? F128_SIGN_MASK : 0)
        | ((u128)(LOG_FRAME_EXP - leading) << F128_EXP_SHIFT)
        + (mantissa - F128_IMPLICIT_BIT) + (v >> 127 != 0));
}

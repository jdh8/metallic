/* Correctly rounded IEEE binary128 logarithms.
 * Ported from metallic-rs 6c2178df4e987cd67e6541bd32843fd3031cb20a.
 * The hot paths use integer fixed point:
 * binary128 multiplication is soft-float on WebAssembly. */
#include "kernel/binary128.h"
#include "kernel/log.h"
#include "kernel/log10tab.h"
#include "kernel/log2tab.h"
#include "kernel/logtab.h"
#include "kernel/uint.h"

#include <stdbool.h>
#include <stdint.h>

typedef __int128 i128;

typedef struct {
    const u128 *per_exponent;
    const u128 (*log0)[3];
    const u128 (*log1)[3];
    const u128 (*log2)[3];
    const u128 (*coef)[2];
    u128 ziv_gate;
} log_base_t;

static const log_base_t LOG_NATURAL = {
    LOG_LN2, LOG0, LOG1, LOG2, LOG_COEF, (u128)1 << 75
};
static const log_base_t LOG_BINARY = {
    LOG2_ONE, LOG2_LOG0, LOG2_LOG1, LOG2_LOG2, LOG2_COEF, (u128)1 << 76
};
static const log_base_t LOG_DECIMAL = {
    LOG10_LOG10_2, LOG10_LOG0, LOG10_LOG1, LOG10_LOG2, LOG10_COEF,
    (u128)1 << 75
};

#define LOG_FAST_FLOOR ((u128)1 << 70)
#define LOG1P_FLOOR ((u128)1 << 67)
#define LOG_ONE_BITS ((u128)F128_BIAS << F128_EXP_SHIFT)
#define LOG1P_SMALL ((u128)(F128_BIAS - 18) << F128_EXP_SHIFT)
#define LOG1P_TINY ((u128)(F128_BIAS - 113) << F128_EXP_SHIFT)
#define LOG_FRAME_EXP (F128_BIAS + 41)
#define LOG1P_SMALL_GATE ((u128)1 << 118)

typedef struct {
    bool present;
    u256_t value;
    unsigned leading;
    bool negative;
} log_tail_t;

static long double log_accurate(const log_base_t *, int, unsigned, u384_t)
    __attribute__((cold, noinline));

static bool u384_zero(u384_t x)
{
    return !(x.limb[0] | x.limb[1] | x.limb[2]);
}

static u128 unsigned_abs_i128(i128 x)
{
    u128 mask = (u128)(x >> 127);
    return ((u128)x ^ mask) - mask;
}

static u128 unsigned_abs_int(int x)
{
    return x < 0 ? (u128)(0u - (unsigned)x) : (u128)(unsigned)x;
}

/* s or -s, in two's-complement limbs, without a data-dependent branch. */
static u256_t negate_if(u256_t s, bool negative)
{
    u128 mask = 0 - (u128)negative;
    u128 flipped = s.limb[0] ^ mask;
    u128 low = flipped + (mask & 1);
    bool carry = low < flipped;
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

static long double log1p_edge(u128 bits)
{
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (magnitude > F128_EXP_MASK)
        return f128_from_bits_(bits | F128_QUIET_BIT);
    if (bits == (F128_SIGN_MASK | LOG_ONE_BITS))
        return f128_from_bits_(F128_SIGN_MASK | F128_EXP_MASK);
    if (bits & F128_SIGN_MASK)
        return f128_from_bits_(F128_EXP_MASK | F128_QUIET_BIT);
    return f128_from_bits_(F128_EXP_MASK);
}

/* floor(2^18 log2(m) + 1/2), accurate enough to choose the reduction. */
static unsigned crude_log2_top(uint64_t h)
{
    uint64_t entry = LOG_CRUDE[(h >> 55) & 255];
    uint64_t fit = (entry >> 23)
        + mul_hi_64_(h << 9, entry & (((uint64_t)1 << 23) - 1));
    return (unsigned)((fit + ((uint64_t)1 << 11)) >> 12);
}

static unsigned crude_log2(u128 m)
{
    return crude_log2_top((uint64_t)(m >> 49));
}

static void log_index(unsigned j, unsigned *j0, unsigned *j1, unsigned *j2)
{
    *j0 = j >> 12;
    *j1 = (j >> 6) & 63;
    *j2 = j & 63;
}

/* 2^(-j/2^18), scaled by 2^93. */
static u128 reciprocal(unsigned j)
{
    unsigned j0, j1, j2;
    log_index(j, &j0, &j1, &j2);
    return (u128)((uint64_t)RECIP0[j0] * RECIP1[j1]) * RECIP2[j2];
}

/* Exact regular-log reduction z, represented at 2^333. */
static u384_t reduce_mantissa(u128 m, unsigned *j)
{
    u128 high, low;
    *j = crude_log2(m);
    wmul_(m, reciprocal(*j), &high, &low);
    return U384(0, low, high - ((u128)1 << 77));
}

static int64_t log_coefficient(const log_base_t *base, unsigned k)
{
    return (int64_t)(base->coef[k][1] >> 64);
}

static int64_t mul_hi_i64(int64_t x, int64_t y)
{
    return (int64_t)((i128)x * y >> 64);
}

static i128 mul_hi_i128(i128 x, u128 y)
{
    u128 sign = (u128)(x >> 127);
    return (i128)(mhi_approx_((u128)x, y) - (sign & y));
}

/* Squaring the magnitude avoids the two's-complement correction underflow
 * when x is tiny and mhi_approx_ has rounded its high half down to zero. */
static u128 sqr_hi(i128 x)
{
    u128 magnitude = unsigned_abs_i128(x);
    return mhi_approx_(magnitude, magnitude);
}

/* log_b(1+z)/z at 2^127, from z at 2^145. */
static u128 log_ratio(const log_base_t *base, i128 z)
{
    i128 narrow = z >> 17;
    int64_t short_z = (int64_t)(z >> 81);
    int64_t tail = log_coefficient(base, 6);

    for (int k = 5; k >= 4; --k)
        tail = log_coefficient(base, (unsigned)k)
            - mul_hi_i64(short_z, tail);

    u128 a = base->coef[0][1]
        - (u128)mul_hi_i128(narrow, base->coef[1][1]);
    u128 b = base->coef[2][1]
        - (u128)mul_hi_i128(narrow, base->coef[3][1]);
    /* The 64-bit tail rides in b as a high-half-only multiplicand
     * (mhi_approx_ folds away the zero low limb), replacing the z^4
     * Estrin leg: two fewer wide multiplies at the same chain depth.
     * The join's short-multiply and tail-lane errors are scaled by
     * z^2 * z^2 < 2^-72, so the fast bound tightens from ~8 to ~7
     * units at 2^127 and every Ziv gate keeps its margin. */
    u128 nn = sqr_hi(narrow);
    b += mhi_approx_(nn, (u128)(uint64_t)tail << 64);
    return a + mhi_approx_(nn, b);
}

/* log_b(1+z) at 2^145. */
static i128 log1p_fast(const log_base_t *base, i128 z)
{
    return (i128)((u128)mul_hi_i128(z, log_ratio(base, z)) << 1);
}

static u256_t log_scale(const log_base_t *base, int e)
{
    u128 a = unsigned_abs_int(e);
    u128 high, low;
    wmul_(a, base->per_exponent[1], &high, &low);
    return negate_if(U256(low,
        high + a * base->per_exponent[2]), e < 0);
}

/* log_b(x) in the fast 2^-214 frame. */
static u256_t log_fast(const log_base_t *base, int e, unsigned j, i128 z)
{
    unsigned j0, j1, j2;
    log_index(j, &j0, &j1, &j2);
    i128 l = log1p_fast(base, z);
    u256_t s = log_scale(base, e);

    s = u256_add_(s, U256(base->log0[j0][1], base->log0[j0][2]));
    s = u256_add_(s, U256(base->log1[j1][1], base->log1[j1][2]));
    s = u256_add_(s, U256(base->log2[j2][1], base->log2[j2][2]));
    return u256_add_(s, U256((u128)l << 69, (u128)(l >> 59)));
}

static i128 z_fast(u384_t d)
{
    return (i128)((d.limb[2] << 68) | (d.limb[1] >> 60));
}

static u384_t log_scale_wide(const log_base_t *base, int e)
{
    u128 a = unsigned_abs_int(e);
    u128 high, low, top, middle;
    wmul_(a, base->per_exponent[0], &high, &low);
    wmul_(a, base->per_exponent[1], &top, &middle);
    u128 sum = middle + high;
    bool carry = sum < middle;
    u384_t product = U384(low, sum,
        top + carry + a * base->per_exponent[2]);
    return e < 0 ? u384_neg_(product) : product;
}

/* High 256 bits of a signed x unsigned 256-bit product. */
static u256_t signed_product(u256_t x, u256_t y, bool negative)
{
    u256_t high = u256_mul_hi_(x, y);
    return negative ? u256_sub_(high, y) : high;
}

/* The only caller shifts by 17: spell it as constants so C never sees an
 * unchecked variable-width shift. */
static u256_t signed_shift_right_17(u256_t x)
{
    return U256((x.limb[0] >> 17) | (x.limb[1] << 111),
        (u128)((i128)x.limb[1] >> 17));
}

/* |log_b(1+z)| at 256 bits. */
static u256_t log1p_wide(const log_base_t *base, u256_t z, u256_t normalized,
    bool negative)
{
    u256_t q = U256(base->coef[13][0], base->coef[13][1]);
    for (int i = 12; i >= 0; --i) {
        u256_t c = U256(base->coef[i][0], base->coef[i][1]);
        q = u256_sub_(c,
            signed_shift_right_17(signed_product(z, q, negative)));
    }
    return u256_mul_hi_(normalized, q);
}

/* Split the accurate result into table terms at 2^342 and a normalized
 * polynomial tail. */
static u384_t log_terms(const log_base_t *base, int e, unsigned j, u384_t d,
    log_tail_t *tail)
{
    unsigned j0, j1, j2;
    log_index(j, &j0, &j1, &j2);
    u384_t s = log_scale_wide(base, e);

    s = u384_add_(s, U384(base->log0[j0][0],
        base->log0[j0][1], base->log0[j0][2]));
    s = u384_add_(s, U384(base->log1[j1][0],
        base->log1[j1][1], base->log1[j1][2]));
    s = u384_add_(s, U384(base->log2[j2][0],
        base->log2[j2][1], base->log2[j2][2]));

    tail->negative = d.limb[2] >> 127;
    u384_t magnitude = tail->negative ? u384_neg_(d) : d;
    if (u384_zero(magnitude)) {
        tail->present = false;
        return s;
    }

    tail->present = true;
    tail->leading = u384_clz_(magnitude);
    u384_t top = u384_shl_(magnitude, tail->leading);
    u256_t z = U256((d.limb[0] >> 60) | (d.limb[1] << 68),
        (d.limb[1] >> 60) | (d.limb[2] << 68));
    tail->value = log1p_wide(base, z, U256(top.limb[1], top.limb[2]),
        tail->negative);
    return s;
}

static u384_t log_tail_frame(log_tail_t tail)
{
    if (tail.leading <= 138)
        return u384_shl_(U384(tail.value.limb[0],
            tail.value.limb[1], 0), 138 - tail.leading);

    u256_t shifted = u256_shr_sat_(tail.value, tail.leading - 138);
    return U384(shifted.limb[0], shifted.limb[1], 0);
}

/* Round a signed 384-bit 2^-342 frame to binary128, ties to even. */
static long double log_round(u384_t s)
{
    bool negative = s.limb[2] >> 127;
    u384_t magnitude = negative ? u384_neg_(s) : s;
    if (u384_zero(magnitude))
        return 0.0L;

    unsigned leading = u384_clz_(magnitude);
    /* Nonzero binary128 logarithms are in [2^-113, 2^14.01], hence
     * leading is in [27,154] and shift in [117,244]. */
    unsigned shift = 271 - leading;
    u128 mantissa = u384_extract_u128_(magnitude, shift);
    unsigned round_position = shift - 1;
    bool round_bit =
        (magnitude.limb[round_position / 128]
            >> (round_position % 128)) & 1;
    bool up = round_bit
        && (u384_any_below_(magnitude, round_position) || (mantissa & 1));

    return f128_from_bits_((negative ? F128_SIGN_MASK : 0)
        | (((u128)(LOG_FRAME_EXP - leading) << F128_EXP_SHIFT)
            + (mantissa - F128_IMPLICIT_BIT) + (u128)up));
}

static long double log_accurate(const log_base_t *base, int e, unsigned j,
    u384_t d)
{
    log_tail_t tail;
    u384_t s = log_terms(base, e, j, d, &tail);
    if (!tail.present)
        return log_round(s);

    u384_t frame = log_tail_frame(tail);
    if (tail.negative)
        frame = u384_neg_(frame);
    return log_round(u384_add_(s, frame));
}

/* The shared fast leg, Ziv gate, and rounding. */
static long double log_finish(const log_base_t *base, int e, unsigned j,
    u384_t d, u128 floor)
{
    u256_t s = log_fast(base, e, j, z_fast(d));
    bool negative = s.limb[1] >> 127;
    u256_t magnitude = negate_if(s, negative);

    if (magnitude.limb[1] < floor)
        return log_accurate(base, e, j, d);

    /* floor pins leading to [27,60], so every 64-bit shift below is safe. */
    unsigned leading =
        __builtin_clzll((uint64_t)(magnitude.limb[1] >> 64));
    unsigned shift = 79 - leading;
    unsigned window = leading - 15;

    u128 v = (u128)funnel_((uint64_t)magnitude.limb[0],
        (uint64_t)(magnitude.limb[0] >> 64), window) << 64;
    v |= (u128)((uint64_t)magnitude.limb[0] << window);

    uint64_t gate = (uint64_t)(base->ziv_gate >> 64);
    u128 bound = (u128)(gate << window) << 64;
    if ((v ^ ((u128)1 << 127)) + bound <= bound << 1)
        return log_accurate(base, e, j, d);

    u128 mantissa = (u128)funnel_down_(
        (uint64_t)magnitude.limb[1],
        (uint64_t)(magnitude.limb[1] >> 64), shift) << 64;
    mantissa |= funnel_down_((uint64_t)(magnitude.limb[0] >> 64),
        (uint64_t)magnitude.limb[1], shift);

    return f128_from_bits_((negative ? F128_SIGN_MASK : 0)
        | (((u128)(LOG_FRAME_EXP - leading) << F128_EXP_SHIFT)
            + (mantissa - F128_IMPLICIT_BIT) + (u128)(v >> 127 != 0)));
}

static long double log_impl(const log_base_t *base, long double x)
{
    u128 bits = f128_bits_(x);
    if (bits - 1 >= F128_EXP_MASK - 1)
        return log_edge(bits);

    u128 m;
    int e;
    f128_split_(bits, &m, &e);
    unsigned j;
    u384_t d = reduce_mantissa(m, &j);
    return log_finish(base, e, j, d, LOG_FAST_FLOOR);
}

/* 1+x as an exact 256-bit significand in [2^255,2^256), plus exponent. */
static u256_t one_plus(u128 m, int e, bool negative, int *result_e)
{
    if (e < 0) {
        /* reduce1p only calls this for e >= -18. */
        u256_t t = u256_shl_(U256(m, 0), (unsigned)(143 + e));
        *result_e = 0;
        if (!negative)
            return u256_add_(U256(0, (u128)1 << 127), t);

        u256_t s = u256_sub_(U256(0, (u128)1 << 127), t);
        unsigned leading = u256_clz_(s);
        *result_e = -(int)leading;
        return u256_shl_(s, leading);
    }

    u256_t one = e < 256
        ? u256_shl_(U256(1, 0), (unsigned)(255 - e))
        : U256(0, 0);
    u128 high = (m << 15) + one.limb[1];
    bool overflow = high < one.limb[1];

    if (overflow) {
        *result_e = e + 1;
        return U256((one.limb[0] >> 1) | (high << 127),
            (high >> 1) | ((u128)1 << 127));
    }
    *result_e = e;
    return U256(one.limb[0], high);
}

/* Exact reduction of 1+x for the general log1p leg. */
static u384_t reduce1p(u128 bits, int *result_e, unsigned *j)
{
    u128 m;
    int e;
    f128_split_(bits & ~F128_SIGN_MASK, &m, &e);
    u256_t big = one_plus(m, e, bits >> 127, result_e);
    *j = crude_log2_top((uint64_t)(big.limb[1] >> 64));
    u128 r = reciprocal(*j);

    u128 h1, l1, h0, l0;
    wmul_(big.limb[1], r, &h1, &l1);
    wmul_(big.limb[0], r, &h0, &l0);
    u128 middle = l1 + h0;
    bool carry = middle < l1;
    u128 top = h1 + carry - ((u128)1 << 92);

    return U384((l0 >> 15) | (middle << 113),
        (middle >> 15) | (top << 113),
        (u128)((i128)top >> 15));
}

static u256_t log1p_small_leg(u128 m, int e, bool negative)
{
    unsigned shift;
    u128 z;
    if (e >= -33) {
        shift = (unsigned)(e + 33);
        z = m << shift;
    } else {
        shift = (unsigned)(-33 - e);
        z = m >> shift;
    }

    u128 mask = 0 - (u128)negative;
    i128 signed_z = (i128)((z ^ mask) - mask);
    u128 high, low;
    wmul_(m << 15, log_ratio(&LOG_NATURAL, signed_z), &high, &low);
    return U256(low, high);
}

static long double log1p_small_accurate(u128 m, int e, bool negative)
    __attribute__((cold, noinline));

static long double log1p_small_accurate(u128 m, int e, bool negative)
{
    /* e >= -113 makes e+161 a guarded [48,142] shift. */
    u256_t z = u256_shl_(U256(m, 0), (unsigned)(e + 161));
    if (negative)
        z = u256_sub_(U256(0, 0), z);

    u256_t w = log1p_wide(&LOG_NATURAL, z, U256(0, m << 15), negative);
    unsigned leading = __builtin_clzll((uint64_t)(w.limb[1] >> 64));
    u256_t n = u256_shl_(w, leading);
    u128 mantissa = n.limb[1] >> 15;
    bool round_bit = (n.limb[1] >> 14) & 1;
    bool sticky = (n.limb[1] & (((u128)1 << 14) - 1)) || n.limb[0];
    bool up = round_bit && (sticky || (mantissa & 1));

    return f128_from_bits_((negative ? F128_SIGN_MASK : 0)
        | (((u128)(e + 1 - (int)leading + F128_BIAS) << F128_EXP_SHIFT)
            + (mantissa - F128_IMPLICIT_BIT) + (u128)up));
}

static long double log1p_small(u128 bits)
{
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (magnitude < LOG1P_TINY)
        return f128_from_bits_(bits);

    u128 m = (magnitude & F128_MANTISSA_MASK) | F128_IMPLICIT_BIT;
    int e = (int)(magnitude >> F128_EXP_SHIFT) - F128_BIAS;
    bool negative = bits >> 127;
    u256_t value = log1p_small_leg(m, e, negative);

    unsigned leading =
        __builtin_clzll((uint64_t)(value.limb[1] >> 64));
    u256_t n = u256_shl_(value, leading);
    u128 v = (n.limb[1] << 113) | (n.limb[0] >> 15);

    if ((v ^ ((u128)1 << 127)) + LOG1P_SMALL_GATE
            <= LOG1P_SMALL_GATE << 1)
        return log1p_small_accurate(m, e, negative);

    return f128_from_bits_((bits & F128_SIGN_MASK)
        | (((u128)(e + 1 - (int)leading + F128_BIAS) << F128_EXP_SHIFT)
            + ((n.limb[1] >> 15) - F128_IMPLICIT_BIT)
            + (u128)(v >> 127 != 0)));
}

long double logl(long double x)
{
    return log_impl(&LOG_NATURAL, x);
}

long double log2l(long double x)
{
    return log_impl(&LOG_BINARY, x);
}

long double log10l(long double x)
{
    return log_impl(&LOG_DECIMAL, x);
}

long double log1pl(long double x)
{
    u128 bits = f128_bits_(x);
    u128 magnitude = bits & ~F128_SIGN_MASK;

    if (magnitude < LOG1P_SMALL)
        return log1p_small(bits);
    if (magnitude >= F128_EXP_MASK
            || bits >= (F128_SIGN_MASK | LOG_ONE_BITS))
        return log1p_edge(bits);

    int e;
    unsigned j;
    u384_t d = reduce1p(bits, &e, &j);
    return log_finish(&LOG_NATURAL, e, j, d, LOG1P_FLOOR);
}

log2_fast_result_ __metallic_log2_fast(u128 mantissa, int exponent)
{
    log2_fast_result_ result;
    result.reduction = reduce_mantissa(mantissa, &result.index);
    u256_t s = log_fast(&LOG_BINARY, exponent, result.index,
        z_fast(result.reduction));
    result.negative = s.limb[1] >> 127;
    result.magnitude = negate_if(s, result.negative);
    return result;
}

static log2_wide_result_ log_wide(const log_base_t *base, int e, unsigned j,
    u384_t d)
{
    log_tail_t tail;
    u384_t s = log_terms(base, e, j, d, &tail);
    log2_wide_result_ result;

    if (u384_zero(s) && tail.present) {
        unsigned shift = u256_clz_(tail.value);
        result.negative = tail.negative;
        result.magnitude = u384_shl_(
            U384(0, tail.value.limb[0], tail.value.limb[1]), shift);
        result.exponent = 51 - (int)tail.leading - (int)shift;
        return result;
    }

    u384_t frame = s;
    if (tail.present) {
        u384_t correction = log_tail_frame(tail);
        frame = u384_add_(frame,
            tail.negative ? u384_neg_(correction) : correction);
    }
    result.negative = frame.limb[2] >> 127;
    u384_t magnitude = result.negative ? u384_neg_(frame) : frame;
    unsigned shift = u384_clz_(magnitude);
    result.magnitude = u384_shl_(magnitude, shift);
    result.exponent = 41 - (int)shift;
    return result;
}

log2_wide_result_ __metallic_log2_wide(int exponent, unsigned index,
    u384_t reduction)
{
    return log_wide(&LOG_BINARY, exponent, index, reduction);
}

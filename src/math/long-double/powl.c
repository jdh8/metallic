/* Correctly rounded binary128 power.
 * Ported from metallic-rs 6c2178df4e987cd67e6541bd32843fd3031cb20a. */
#include "kernel/binary128.h"
#include "kernel/exp_engine.h"
#include "kernel/log.h"
#include "kernel/powtab.h"
#include "kernel/uint.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define POW_ONE ((u128)F128_BIAS << F128_EXP_SHIFT)
#define POW_ACCURATE_GATE ((u128)1 << 20)
#define POW_LIMBS 10
#define POW_WIDE_LIMBS 12

typedef struct {
    uint64_t limb[POW_LIMBS];
} pow_big_t;

typedef struct {
    uint64_t limb[POW_WIDE_LIMBS];
} pow_wide_t;

static unsigned pow_clz128_(u128 x)
{
    uint64_t high = x >> 64;
    uint64_t low = x;
    return high ? __builtin_clzll(high) : low ? 64 + __builtin_clzll(low) : 128;
}

static unsigned pow_ctz128_(u128 x)
{
    uint64_t low = x;
    return low ? __builtin_ctzll(low) : 64 + __builtin_ctzll((uint64_t)(x >> 64));
}

static u128 pow_abs_diff_(u128 a, u128 b)
{
    return a > b ? a - b : b - a;
}

static pow_big_t pow_big_zero_(void)
{
    return (pow_big_t){{0}};
}

static bool pow_big_zero_p_(pow_big_t a)
{
    for (unsigned i = 0; i < POW_LIMBS; ++i)
        if (a.limb[i])
            return false;
    return true;
}

static pow_big_t pow_big_add_(pow_big_t a, pow_big_t b)
{
    pow_big_t out;
    u128 carry = 0;
    for (unsigned i = 0; i < POW_LIMBS; ++i) {
        u128 sum = (u128)a.limb[i] + b.limb[i] + carry;
        out.limb[i] = sum;
        carry = sum >> 64;
    }
    return out;
}

static pow_big_t pow_big_sub_(pow_big_t a, pow_big_t b)
{
    pow_big_t out;
    u128 borrow = 0;
    for (unsigned i = 0; i < POW_LIMBS; ++i) {
        u128 difference = ((u128)a.limb[i] | (u128)1 << 64)
            - b.limb[i] - borrow;
        out.limb[i] = difference;
        borrow = difference >> 64 == 0;
    }
    return out;
}

static pow_big_t pow_big_neg_(pow_big_t a)
{
    return pow_big_sub_(pow_big_zero_(), a);
}

static pow_big_t pow_big_shl_(pow_big_t a, unsigned shift)
{
    pow_big_t out = {{0}};
    if (shift >= 64 * POW_LIMBS)
        return out;
    unsigned word = shift / 64;
    unsigned bits = shift % 64;
    for (unsigned i = POW_LIMBS; i-- > word;) {
        uint64_t carry = bits && i > word
            ? a.limb[i - word - 1] >> (64 - bits) : 0;
        out.limb[i] = a.limb[i - word] << bits | carry;
    }
    return out;
}

static pow_big_t pow_big_shr_(pow_big_t a, unsigned shift)
{
    pow_big_t out = {{0}};
    if (shift >= 64 * POW_LIMBS)
        return out;
    unsigned word = shift / 64;
    unsigned bits = shift % 64;
    for (unsigned i = word; i < POW_LIMBS; ++i) {
        uint64_t carry = bits && i + 1 < POW_LIMBS
            ? a.limb[i + 1] << (64 - bits) : 0;
        out.limb[i - word] = a.limb[i] >> bits | carry;
    }
    return out;
}

static pow_wide_t pow_wide_shl_(pow_wide_t a, unsigned shift)
{
    pow_wide_t out = {{0}};
    if (shift >= 64 * POW_WIDE_LIMBS)
        return out;
    unsigned word = shift / 64;
    unsigned bits = shift % 64;
    for (unsigned i = POW_WIDE_LIMBS; i-- > word;) {
        uint64_t carry = bits && i > word
            ? a.limb[i - word - 1] >> (64 - bits) : 0;
        out.limb[i] = a.limb[i - word] << bits | carry;
    }
    return out;
}

static pow_wide_t pow_wide_shr_(pow_wide_t a, unsigned shift)
{
    pow_wide_t out = {{0}};
    if (shift >= 64 * POW_WIDE_LIMBS)
        return out;
    unsigned word = shift / 64;
    unsigned bits = shift % 64;
    for (unsigned i = word; i < POW_WIDE_LIMBS; ++i) {
        uint64_t carry = bits && i + 1 < POW_WIDE_LIMBS
            ? a.limb[i + 1] << (64 - bits) : 0;
        out.limb[i - word] = a.limb[i] >> bits | carry;
    }
    return out;
}

static bool pow_wide_any_below_(pow_wide_t a, unsigned n)
{
    if (n >= 64 * POW_WIDE_LIMBS) {
        for (unsigned i = 0; i < POW_WIDE_LIMBS; ++i)
            if (a.limb[i])
                return true;
        return false;
    }
    unsigned word = n / 64;
    unsigned bits = n % 64;
    for (unsigned i = 0; i < word; ++i)
        if (a.limb[i])
            return true;
    return bits && (a.limb[word] & (UINT64_MAX >> (64 - bits)));
}

static pow_big_t pow_big_mul_hi_(pow_big_t a, pow_big_t b)
{
    uint64_t p[2 * POW_LIMBS] = {0};
    for (unsigned i = 0; i < POW_LIMBS; ++i) {
        u128 carry = 0;
        for (unsigned j = 0; j < POW_LIMBS; ++j) {
            u128 t = (u128)a.limb[i] * b.limb[j] + p[i + j] + carry;
            p[i + j] = t;
            carry = t >> 64;
        }
        p[i + POW_LIMBS] = carry;
    }
    pow_big_t out;
    for (unsigned i = 0; i < POW_LIMBS; ++i)
        out.limb[i] = p[i + POW_LIMBS];
    return out;
}

static pow_wide_t pow_big_mul_u128_(pow_big_t a, u128 m)
{
    uint64_t b[2] = {(uint64_t)m, (uint64_t)(m >> 64)};
    pow_wide_t out = {{0}};
    for (unsigned i = 0; i < POW_LIMBS; ++i) {
        u128 carry = 0;
        for (unsigned j = 0; j < 2; ++j) {
            u128 t = (u128)a.limb[i] * b[j] + out.limb[i + j] + carry;
            out.limb[i + j] = t;
            carry = t >> 64;
        }
        out.limb[i + 2] = carry;
    }
    return out;
}

static pow_big_t pow_big_div_small_(pow_big_t a, uint64_t divisor)
{
    pow_big_t out;
    u128 rest = 0;
    for (unsigned i = POW_LIMBS; i-- > 0;) {
        u128 current = rest << 64 | a.limb[i];
        out.limb[i] = current / divisor;
        rest = current % divisor;
    }
    return out;
}

static pow_wide_t pow_wide_negate_(pow_wide_t a, bool negative, bool dropped)
{
    if (!negative)
        return a;
    pow_wide_t out;
    u128 carry = !dropped;
    for (unsigned i = 0; i < POW_WIDE_LIMBS; ++i) {
        u128 sum = (u128)~a.limb[i] + carry;
        out.limb[i] = sum;
        carry = sum >> 64;
    }
    return out;
}

static pow_big_t pow_big_from_384_(u384_t x)
{
    pow_big_t out = {{0}};
    for (unsigned i = 0; i < 3; ++i) {
        out.limb[2 * i] = x.limb[i];
        out.limb[2 * i + 1] = x.limb[i] >> 64;
    }
    return out;
}

static pow_big_t pow_big_constant_(const uint64_t value[POW_LIMBS])
{
    pow_big_t out;
    for (unsigned i = 0; i < POW_LIMBS; ++i)
        out.limb[i] = value[i];
    return out;
}

static pow_big_t pow_big_one_(void)
{
    pow_big_t one = {{0}};
    one.limb[POW_LIMBS - 1] = (uint64_t)1 << 63;
    return one;
}

/* 2^f for f in [0, 1), at 640 bits.  This is the cold final Ziv tier. */
static pow_big_t pow_exp2_wide_(pow_big_t fraction)
{
    pow_big_t t = pow_big_shr_(pow_big_mul_hi_(fraction,
        pow_big_constant_(POW_LN2)), 16);
    pow_big_t term = pow_big_shr_(t, 1);
    pow_big_t sum = pow_big_add_(pow_big_one_(), term);

    for (uint64_t k = 2; !pow_big_zero_p_(term); ++k) {
        term = pow_big_div_small_(pow_big_mul_hi_(term, t), k);
        sum = pow_big_add_(sum, term);
    }
    for (unsigned i = 0; i < 16; ++i)
        sum = pow_big_shl_(pow_big_mul_hi_(sum, sum), 1);
    return sum;
}

static long double pow_with_sign_(long double magnitude, u128 sign)
{
    return f128_from_bits_(f128_bits_(magnitude) | sign);
}

/* Whether finite nonzero |y| is an integer, and its parity when it is. */
static bool pow_parity_(u128 ay, bool *odd)
{
    int exponent = (int)(ay >> F128_EXP_SHIFT) - F128_BIAS;
    if (exponent < 0)
        return false;
    if (exponent >= 113) {
        *odd = false;
        return true;
    }
    u128 mantissa = (ay & F128_MANTISSA_MASK) | F128_IMPLICIT_BIT;
    unsigned below = 112 - (unsigned)exponent;
    if (mantissa & (((u128)1 << below) - 1))
        return false;
    *odd = (mantissa >> below) & 1;
    return true;
}

static __attribute__((cold, noinline)) long double pow_special_(u128 xb, u128 yb)
{
    u128 ax = xb & ~F128_SIGN_MASK;
    u128 ay = yb & ~F128_SIGN_MASK;
    if (ax > F128_EXP_MASK || ay > F128_EXP_MASK)
        return f128_from_bits_((ax > F128_EXP_MASK ? xb : yb) | F128_QUIET_BIT);

    bool negative_y = yb >> 127;
    if (ay == F128_EXP_MASK) {
        if (ax == POW_ONE)
            return 1.0L;
        return (ax < POW_ONE) == negative_y
            ? f128_from_bits_(F128_EXP_MASK) : 0.0L;
    }

    bool odd;
    u128 sign = pow_parity_(ay, &odd) && odd ? xb & F128_SIGN_MASK : 0;
    bool infinite = (ax == 0) == negative_y;
    return f128_from_bits_(sign | (infinite ? F128_EXP_MASK : 0));
}

static u128 pow_isqrt_(u128 n)
{
    unsigned width = 128 - pow_clz128_(n);
    u128 root = (u128)1 << ((width + 1) / 2);
    for (;;) {
        u128 next = (root + n / root) >> 1;
        if (next >= root)
            return root;
        root = next;
    }
}

static bool pow_checked_power_(u128 base, unsigned exponent, u128 *result)
{
    u128 value = 1;
    u128 square = base;
    for (;;) {
        if (exponent & 1) {
            if (__builtin_mul_overflow(value, square, &value)
                    || value >= (u128)1 << 114)
                return false;
        }
        exponent >>= 1;
        if (!exponent) {
            *result = value;
            return true;
        }
        if (__builtin_mul_overflow(square, square, &square))
            return false;
    }
}

static long double pow_scaled_(u128 k, int64_t scale)
{
    unsigned width = 128 - pow_clz128_(k);
    int64_t exponent = scale + width - 1;
    if (exponent < -(1 << 20))
        exponent = -(1 << 20);
    if (exponent > 1 << 20)
        exponent = 1 << 20;
    return __metallic_f128_exp_round((int)exponent, k << (128 - width), 0);
}

static long double pow_power_of_two_(u128 magnitude, bool negative)
{
    int exponent = magnitude > (u128)1 << 20 ? 1 << 20 : (int)magnitude;
    return __metallic_f128_exp_round(negative ? -exponent : exponent,
        (u128)1 << 127, 0);
}

/* Return true when |x|^|y| is in the dyadic exact family. */
static bool pow_exact_(u128 ax, u128 ay, bool negative_y, long double *result)
{
    u128 m;
    u128 n;
    int e;
    int f;
    f128_split_(ax, &m, &e);
    f128_split_(ay, &n, &f);

    unsigned zeros = pow_ctz128_(m);
    m >>= zeros;
    e += (int)zeros - 112;
    zeros = pow_ctz128_(n);
    n >>= zeros;
    f += (int)zeros - 112;

    if (m == 1) {
        u128 magnitude;
        if (f >= 0) {
            if (f > 21) {
                *result = pow_power_of_two_(~(u128)0, (e < 0) != negative_y);
                return true;
            }
            magnitude = (u128)(e < 0 ? -(int64_t)e : e) * n;
            if (magnitude > (~(u128)0 >> f))
                magnitude = ~(u128)0;
            else
                magnitude <<= f;
        } else {
            unsigned shift = -f;
            if (__builtin_ctz((unsigned)e) < shift)
                return false;
            magnitude = (u128)(e < 0 ? -(int64_t)e : e) >> shift;
            magnitude *= n;
        }
        *result = pow_power_of_two_(magnitude, (e < 0) != negative_y);
        return true;
    }
    if (negative_y || n > 71)
        return false;

    unsigned numerator = n;
    if (f >= 0) {
        if (f > 6 || (numerator << f) > 71)
            return false;
        unsigned y = numerator << f;
        u128 value;
        if (!pow_checked_power_(m, y, &value))
            return false;
        *result = pow_scaled_(value, (int64_t)e * y);
        return true;
    }

    unsigned k = -f;
    if (k > 6 || ((unsigned)e & ((1u << k) - 1)))
        return false;
    u128 root = m;
    for (unsigned i = 0; i < k; ++i) {
        u128 candidate = pow_isqrt_(root);
        if (candidate * candidate != root)
            return false;
        root = candidate;
    }
    u128 value;
    if (!pow_checked_power_(root, numerator, &value))
        return false;
    *result = pow_scaled_(value, (int64_t)(e >> k) * numerator);
    return true;
}

static u512_t pow_u512_shr_sat_(u512_t x, unsigned shift)
{
    u512_t out = {{0}};
    if (shift >= 512)
        return out;
    unsigned word = shift / 128;
    unsigned bits = shift % 128;
    for (unsigned i = word; i < 4; ++i) {
        u128 carry = bits && i + 1 < 4 ? x.limb[i + 1] << (128 - bits) : 0;
        out.limb[i - word] = x.limb[i] >> bits | carry;
    }
    return out;
}

static bool pow_u512_any_below_(u512_t x, unsigned n)
{
    if (n >= 512)
        return x.limb[0] || x.limb[1] || x.limb[2] || x.limb[3];
    unsigned word = n / 128;
    unsigned bits = n % 128;
    for (unsigned i = 0; i < word; ++i)
        if (x.limb[i])
            return true;
    return bits && (x.limb[word] & (~(u128)0 >> (128 - bits)));
}

static void pow_frame_(u128 fraction, u128 integer, bool negative,
    bool dropped, u128 *out_fraction, int *out_integer)
{
    u128 mask = 0 - (u128)negative;
    u128 before = fraction ^ mask;
    u128 adjusted = before + (mask & (u128)!dropped);
    bool carry = adjusted < before;
    *out_fraction = adjusted;
    *out_integer = (int)(uint32_t)((integer ^ mask) + carry);
}

static f128_exp_fast_result_t pow_fast_(u256_t logarithm, u128 mantissa,
    int exponent, bool negative)
{
    u384_t product = wmul_128x256_(mantissa, logarithm);
    unsigned shift = 198 - exponent;
    u384_t window = u384_shr_sat_(product, shift);
    u128 fraction;
    int integer;
    pow_frame_(window.limb[0], window.limb[1], negative,
        u384_any_below_(product, shift), &fraction, &integer);
    return __metallic_f128_exp_fast(integer, fraction);
}

static u128 pow_extra_gate_(u128 mantissa, int exponent)
{
    if (exponent >= 25)
        return (u128)1 << 14;
    unsigned shift = 123 - exponent;
    return (shift >= 128 ? 0 : mantissa >> shift) + 1;
}

static u384_t pow_negate_frame_(u384_t x, bool negative, bool dropped)
{
    if (!negative)
        return x;
    u128 low = ~x.limb[0] + !dropped;
    bool carry = low < ~x.limb[0];
    u128 middle = ~x.limb[1] + carry;
    carry = middle < ~x.limb[1];
    return U384(low, middle, ~x.limb[2] + carry);
}

static bool pow_accurate_undecided_(int exponent, u256_t significand)
{
    int subnormal = -16382 - exponent;
    unsigned shift = 143 + (subnormal > 0 ? (unsigned)subnormal : 0);
    if (exponent > 16383 || shift > 256)
        return false;

    u256_t field;
    if (shift == 256) {
        field = significand;
    } else if (shift > 128) {
        field = U256(significand.limb[0],
            significand.limb[1] & (~(u128)0 >> (256 - shift)));
    } else {
        field = U256(significand.limb[0]
            & (~(u128)0 >> (128 - shift)), 0);
    }
    u256_t distance = u256_sub_(field,
        u256_shl_(U256(1, 0), shift - 1));
    if (distance.limb[1] >> 127)
        distance = u256_sub_(U256(0, 0), distance);
    return distance.limb[1] == 0 && distance.limb[0] <= POW_ACCURATE_GATE;
}

typedef struct {
    f128_exp_wide_result_t exponential;
    log2_wide_result_ logarithm;
} pow_accurate_result_t;

static pow_accurate_result_t pow_accurate_leg_(int exponent, unsigned index,
    u384_t reduction, u128 ym, int ye, bool negative_y)
{
    log2_wide_result_ logarithm = __metallic_log2_wide(exponent, index,
        reduction);
    bool negative = negative_y != logarithm.negative;
    u512_t product = wmul_128x384_(ym, logarithm.magnitude);
    unsigned shift = 239 - ye - logarithm.exponent;
    u512_t window = pow_u512_shr_sat_(product, shift);
    u384_t frame = U384(window.limb[0], window.limb[1], window.limb[2]);
    frame = pow_negate_frame_(frame, negative,
        pow_u512_any_below_(product, shift));
    return (pow_accurate_result_t) {
        .exponential = __metallic_f128_exp2_frame(frame),
        .logarithm = logarithm,
    };
}

static long double pow_wide_(u128, int, u128, int, bool, log2_wide_result_);

static __attribute__((cold, noinline)) long double pow_exact_or_wide_(u128 ax,
    u128 ay, bool negative_y, u128 ym, int ye, log2_wide_result_ logarithm)
{
    long double exact;
    if (pow_exact_(ax, ay, negative_y, &exact))
        return exact;
    u128 m;
    int exponent;
    f128_split_(ax, &m, &exponent);
    return pow_wide_(m, exponent, ym, ye, negative_y, logarithm);
}

static __attribute__((cold, noinline)) long double pow_accurate_(int exponent,
    unsigned index, u384_t reduction, u128 ym, int ye, bool negative_y,
    u128 ax, u128 ay)
{
    pow_accurate_result_t result = pow_accurate_leg_(exponent, index,
        reduction, ym, ye, negative_y);
    if (pow_accurate_undecided_(result.exponential.n,
            result.exponential.r))
        return pow_exact_or_wide_(ax, ay, negative_y, ym, ye,
            result.logarithm);
    return __metallic_f128_exp_round(result.exponential.n,
        result.exponential.r.limb[1], result.exponential.r.limb[0]);
}

static pow_big_t pow_big_from_wide_(pow_wide_t x)
{
    pow_big_t out;
    for (unsigned i = 0; i < POW_LIMBS; ++i)
        out.limb[i] = x.limb[i];
    return out;
}

static long double pow_wide_(u128 m, int exponent, u128 ym, int ye,
    bool negative_y, log2_wide_result_ logarithm)
{
    /* L0 = the accurate logarithm in a 2^-620 frame. */
    pow_big_t l0 = pow_big_shl_(pow_big_from_384_(logarithm.magnitude),
        logarithm.exponent + 237);
    if (logarithm.negative)
        l0 = pow_big_neg_(l0);

    pow_big_t minus = pow_big_neg_(l0);
    int64_t integer = (int64_t)minus.limb[POW_LIMBS - 1] >> 44;
    pow_big_t fraction = pow_big_shl_(minus, 20);
    pow_big_t exponential = pow_exp2_wide_(fraction);

    /* x*2^-L0 - 1: the low 640 bits around the binary point. */
    pow_wide_t product = pow_big_mul_u128_(exponential, m);
    unsigned shift = 111 - exponent - (int)integer;
    pow_big_t epsilon = pow_big_from_wide_(pow_wide_shr_(product, shift));
    bool negative_epsilon = epsilon.limb[POW_LIMBS - 1] >> 63;
    pow_big_t magnitude = negative_epsilon ? pow_big_neg_(epsilon) : epsilon;

    pow_big_t log2e = pow_big_constant_(POW_LOG2E);
    pow_big_t c1 = pow_big_shr_(pow_big_mul_hi_(magnitude, log2e), 19);
    pow_big_t square = pow_big_mul_hi_(magnitude, magnitude);
    pow_big_t c2 = pow_big_shr_(pow_big_mul_hi_(square, log2e), 20);
    pow_big_t logarithm_refined = pow_big_add_(l0,
        negative_epsilon ? pow_big_neg_(c1) : c1);
    logarithm_refined = pow_big_sub_(logarithm_refined, c2);
    bool negative_logarithm = logarithm_refined.limb[POW_LIMBS - 1] >> 63;
    if (negative_logarithm)
        logarithm_refined = pow_big_neg_(logarithm_refined);
    bool negative = negative_y != negative_logarithm;

    product = pow_big_mul_u128_(logarithm_refined, ym);
    bool dropped = false;
    pow_wide_t window;
    if (ye <= 92) {
        shift = 92 - ye;
        dropped = pow_wide_any_below_(product, shift);
        window = pow_wide_shr_(product, shift);
    } else {
        window = pow_wide_shl_(product, ye - 92);
    }
    window = pow_wide_negate_(window, negative, dropped);
    int n = (int)(int32_t)window.limb[POW_LIMBS];
    fraction = pow_big_from_wide_(window);
    pow_big_t result = pow_exp2_wide_(fraction);
    u128 high = (u128)result.limb[POW_LIMBS - 1] << 64
        | result.limb[POW_LIMBS - 2];
    u128 sticky = 0;
    for (unsigned i = 0; i < POW_LIMBS - 2; ++i)
        sticky |= result.limb[i];
    return __metallic_f128_exp_round(n, high, sticky != 0);
}

long double powl(long double x, long double y)
{
    u128 xb = f128_bits_(x);
    u128 yb = f128_bits_(y);
    u128 ax = xb & ~F128_SIGN_MASK;
    u128 ay = yb & ~F128_SIGN_MASK;

    if (ay == 0 || xb == POW_ONE)
        return 1.0L;
    if (ax - 1 >= F128_EXP_MASK - 1 || ay >= F128_EXP_MASK)
        return pow_special_(xb, yb);

    u128 sign = 0;
    if (xb >> 127) {
        bool odd;
        if (!pow_parity_(ay, &odd))
            return f128_from_bits_(F128_EXP_MASK | F128_QUIET_BIT);
        sign = (u128)odd << 127;
    }

    u128 m;
    u128 ym;
    int exponent;
    int ye;
    f128_split_(ax, &m, &exponent);
    f128_split_(ay, &ym, &ye);
    bool negative_y = yb >> 127;
    log2_fast_result_ logarithm = __metallic_log2_fast(m, exponent);

    if (!(logarithm.magnitude.limb[0] | logarithm.magnitude.limb[1]))
        return pow_with_sign_(1.0L, sign);
    bool negative = negative_y != logarithm.negative;
    int top = ye + 43 - (int)u256_clz_(logarithm.magnitude);
    if (top >= 17)
        return pow_with_sign_(negative ? 0.0L
            : f128_from_bits_(F128_EXP_MASK), sign);
    if (top <= -120)
        return pow_with_sign_(1.0L, sign);

    f128_exp_fast_result_t fast = pow_fast_(logarithm.magnitude, ym, ye,
        negative);
    u128 gate = F128_EXP_ZIV_GATE + pow_extra_gate_(ym, ye);
    if ((unsigned)(fast.n + 16382) > 32765u) {
        if (__metallic_f128_exp_undecided(fast.n, fast.r, gate))
            return pow_with_sign_(pow_accurate_(exponent, logarithm.index,
                logarithm.reduction, ym, ye, negative_y, ax, ay), sign);
        return pow_with_sign_(__metallic_f128_exp_round(fast.n, fast.r, 0),
            sign);
    }

    u128 rest = fast.r & F128_EXP_GUARD_MASK;
    if (pow_abs_diff_(rest, F128_EXP_GUARD_HALF) <= gate)
        return pow_with_sign_(pow_accurate_(exponent, logarithm.index,
            logarithm.reduction, ym, ye, negative_y, ax, ay), sign);
    return pow_with_sign_(f128_from_bits_(
        ((u128)(fast.n + F128_BIAS) << F128_EXP_SHIFT)
        + ((fast.r >> F128_EXP_GUARD) - F128_IMPLICIT_BIT)
        + (rest > F128_EXP_GUARD_HALF)), sign);
}

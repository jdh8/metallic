#ifndef METALLIC_MATH_LONG_DOUBLE_KERNEL_TRIG_H
#define METALLIC_MATH_LONG_DOUBLE_KERNEL_TRIG_H

#include "atan.h"
#include "trigtab.h"
#include "uint.h"
#include <stdbool.h>
#include <stdint.h>

#define TRIG_TINY_ ((u128)(F128_BIAS - 57) << F128_EXP_SHIFT)
#define TRIG_DIRECT_ (-8)
#define TRIG_MAX_LZ_ 56

typedef struct {
    unsigned n;
    bool negative;
    u128 fraction;
    int exponent;
} trig_residual_t;

typedef struct {
    unsigned n;
    bool negative;
    u384_t fraction;
    int exponent;
} trig_residual_wide_t;

typedef struct {
    u128 square;
    u128 fourth;
    u128 unshifted;
} trig_squares_t;

static __attribute__((cold, noinline)) long double
trig_edge_(u128 bits, u128 magnitude)
{
    return magnitude > F128_EXP_MASK
        ? f128_from_bits_(bits | F128_QUIET_BIT)
        : f128_from_bits_(F128_EXP_MASK | F128_QUIET_BIT);
}

static inline uint64_t trig_window_limb_(const uint64_t *window,
    unsigned width, unsigned index)
{
    return index < width ? window[width - 1 - index] : 0;
}

/* m times a most-significant-first 2/pi window, returned in little-endian
 * 64-bit limbs.  The caller leaves two product limbs and one zero top limb. */
static inline void trig_product_(u128 m, const uint64_t *window,
    unsigned width, uint64_t *product)
{
    uint64_t low = (uint64_t)m;
    uint64_t high = (uint64_t)(m >> 64);
    u128 carry = 0;

    for (unsigned i = 0; i < width + 3; ++i) {
        u128 sum = umulditi3_(high,
            trig_window_limb_(window, width, i ? i - 1 : width)) + carry;
        u128 addend = umulditi3_(low,
            trig_window_limb_(window, width, i));
        u128 next = sum + addend;
        bool spill = next < sum;
        product[i] = (uint64_t)next;
        carry = (next >> 64) | (u128)spill << 64;
    }
}

static inline const uint64_t *trig_window_(int exponent, int *shift)
{
    int first = exponent - 114;
    if (first < 0) {
        *shift = first;
        return trig_two_over_pi_;
    }
    *shift = first % 64;
    return trig_two_over_pi_ + first / 64;
}

/* x >> shift, rounded on the bit immediately below, for 0 < shift < 128. */
static inline u128 trig_shr_round_(u128 x, unsigned shift)
{
    if (!shift)
        return x;
    if (shift >= 128)
        return 0;

    unsigned bits = shift & 63;
    uint64_t low = shift < 64 ? (uint64_t)x : (uint64_t)(x >> 64);
    uint64_t high = shift < 64 ? (uint64_t)(x >> 64) : 0;
    unsigned below = shift - 1;
    uint64_t word = below < 64 ? (uint64_t)x : (uint64_t)(x >> 64);
    uint64_t bit = word >> (below & 63) & 1;
    return (u128)funnel_down_(low, high, bits)
        | (u128)(high >> bits) << 64 | bit;
}

/* fraction << shift in a 256-bit little-endian frame. */
static inline u256_t trig_place_256_(u128 fraction, unsigned shift)
{
    if (shift >= 256)
        return U256(0, 0);
    if (shift >= 128)
        return U256(0, fraction << (shift - 128));

    unsigned bits = shift & 63;
    uint64_t a = shift < 64 ? (uint64_t)fraction : 0;
    uint64_t b = shift < 64 ? (uint64_t)(fraction >> 64) : (uint64_t)fraction;
    uint64_t c = shift < 64 ? 0 : (uint64_t)(fraction >> 64);
    return U256((u128)(a << bits) | (u128)funnel_(a, b, bits) << 64,
        (u128)funnel_(b, c, bits) | (u128)funnel_(c, 0, bits) << 64);
}

/* Top 128 bits of value << shift.  Every caller normalizes by < 64 bits. */
static inline u128 trig_top_256_(u256_t value, unsigned shift)
{
    uint64_t low = (uint64_t)(value.limb[0] >> 64);
    uint64_t middle = (uint64_t)value.limb[1];
    uint64_t high = (uint64_t)(value.limb[1] >> 64);
    return (u128)funnel_(low, middle, shift)
        | (u128)funnel_(middle, high, shift) << 64;
}

static inline u256_t trig_add_signed_256_(u256_t a, u256_t b, bool negative)
{
    u128 mask = 0 - (u128)negative;
    return u256_add_(a, u256_add_(
        U256(b.limb[0] ^ mask, b.limb[1] ^ mask), U256(negative, 0)));
}

static inline bool trig_round_fast_(u128 fraction, int exponent,
    u128 sign, long double *result)
{
    if (exponent < -16381)
        return false;

    u128 rest = fraction & ATAN_GUARD_MASK_;
    u128 distance = rest > ATAN_GUARD_HALF_
        ? rest - ATAN_GUARD_HALF_ : ATAN_GUARD_HALF_ - rest;
    if (distance <= ATAN_ZIV_GATE)
        return false;

    u128 bits = ((u128)(exponent + 16382) << F128_EXP_SHIFT)
        + (fraction >> ATAN_GUARD_) - F128_IMPLICIT_BIT
        + (rest > ATAN_GUARD_HALF_);
    *result = f128_from_bits_(sign | bits);
    return true;
}

/* Six-limb Payne-Hanek reduction.  False hands a deep cancellation to the
 * wide reduction rather than evaluating it with fewer than 136 useful bits. */
static inline bool trig_reduce_(u128 mantissa, int exponent,
    trig_residual_t *result)
{
    int shift;
    const uint64_t *window = trig_window_(exponent, &shift);
    uint64_t product[9];
    trig_product_(mantissa, window, 6, product);

    unsigned base = (unsigned)(190 - shift);
    unsigned index = base / 64;
    unsigned bits = base % 64;
    uint64_t f0 = funnel_down_(product[index], product[index + 1], bits);
    uint64_t f1 = funnel_down_(product[index + 1], product[index + 2], bits);
    uint64_t f2 = funnel_down_(product[index + 2], product[index + 3], bits);
    uint64_t quadrant = funnel_down_(product[index + 3],
        product[index + 4], bits) & 3;
    u128 top = (u128)quadrant << 64 | f2;
    unsigned n = (unsigned)((top + ((u128)1 << 56)) >> 57) & 511;
    f2 -= (uint64_t)n << 57;

    bool negative = (int64_t)f2 < 0;
    uint64_t mask = 0 - (uint64_t)negative;
    uint64_t g0 = (f0 ^ mask) + negative;
    bool carry0 = g0 < (f0 ^ mask);
    uint64_t g1 = (f1 ^ mask) + carry0;
    bool carry1 = g1 < (f1 ^ mask);
    uint64_t g2 = (f2 ^ mask) + carry1;
    unsigned leading = g2 ? (unsigned)__builtin_clzll(g2) : 64;
    if (leading > TRIG_MAX_LZ_)
        return false;

    uint64_t high;
    uint64_t low;
    if (leading) {
        high = g2 << leading | g1 >> (64 - leading);
        low = g1 << leading | g0 >> (64 - leading);
    } else {
        high = g2;
        low = g1;
    }
    u128 g = (u128)high << 64 | low;
    u128 ph;
    u128 pl;
    wmul_(g, trig_pio2_128_, &ph, &pl);
    unsigned product_leading = (unsigned)clzti2_(ph);

    result->n = n;
    result->negative = negative;
    result->fraction = trig_top_256_(U256(pl, ph), product_leading);
    result->exponent = 1 - (int)(leading + product_leading);
    return true;
}

static inline void trig_shl_limbs_(const uint64_t input[7],
    unsigned shift, uint64_t result[7])
{
    for (unsigned i = 0; i < 7; ++i)
        result[i] = 0;

    unsigned word = shift / 64;
    unsigned bits = shift % 64;
    if (word >= 7)
        return;
    for (unsigned i = word; i < 7; ++i) {
        uint64_t carry = bits && i > word
            ? input[i - word - 1] >> (64 - bits) : 0;
        result[i] = input[i - word] << bits | carry;
    }
}

/* Nine-limb Payne-Hanek reduction for the cold 384-bit path. */
static trig_residual_wide_t trig_reduce_wide_(u128 mantissa, int exponent)
{
    int shift;
    const uint64_t *window = trig_window_(exponent, &shift);
    uint64_t product[12];
    uint64_t fraction[7];
    trig_product_(mantissa, window, 9, product);

    unsigned base = (unsigned)(126 - shift);
    unsigned index = base / 64 & 3;
    unsigned bits = base % 64;
    for (unsigned i = 0; i < 7; ++i)
        fraction[i] = funnel_down_(product[index + i],
            product[index + i + 1], bits);
    uint64_t quadrant = funnel_down_(product[index + 7],
        product[index + 8], bits) & 3;
    u128 top = (u128)quadrant << 64 | fraction[6];
    unsigned n = (unsigned)((top + ((u128)1 << 56)) >> 57) & 511;
    fraction[6] -= (uint64_t)n << 57;

    bool negative = (int64_t)fraction[6] < 0;
    uint64_t mask = 0 - (uint64_t)negative;
    bool carry = negative;
    for (unsigned i = 0; i < 7; ++i) {
        uint64_t before = fraction[i] ^ mask;
        fraction[i] = before + carry;
        carry = fraction[i] < before;
    }

    unsigned leading = 448;
    for (unsigned i = 7; i-- > 0;) {
        if (fraction[i]) {
            leading = (6 - i) * 64 + (unsigned)__builtin_clzll(fraction[i]);
            break;
        }
    }
    uint64_t normalized[7];
    trig_shl_limbs_(fraction, leading, normalized);
    u384_t g = U384((u128)normalized[1] | (u128)normalized[2] << 64,
        (u128)normalized[3] | (u128)normalized[4] << 64,
        (u128)normalized[5] | (u128)normalized[6] << 64);
    u384_t angle = u384_mul_hi_(g, trig_pio2_384_);
    unsigned angle_leading = u384_clz_(angle);

    return (trig_residual_wide_t) {
        .n = n,
        .negative = negative,
        .fraction = u384_shl_(angle, angle_leading),
        .exponent = 1 - (int)(leading + angle_leading),
    };
}

static inline trig_squares_t trig_squares_(u128 fraction, int exponent)
{
    u128 unshifted = mhi_approx_(fraction, fraction);
    u128 square = trig_shr_round_(unshifted, (unsigned)(-2 * exponent));
    return (trig_squares_t) {
        .square = square,
        .fourth = mhi_approx_(square, square),
        .unshifted = unshifted,
    };
}

#endif

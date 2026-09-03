#ifndef METALLIC_MATH_LONG_DOUBLE_KERNEL_UINT_H
#define METALLIC_MATH_LONG_DOUBLE_KERNEL_UINT_H

#include "binary128.h"
#include "../../../soft/integer/umulti4.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    u128 limb[2];
} u256_t;

typedef struct {
    u128 limb[3];
} u384_t;

typedef struct {
    u128 limb[4];
} u512_t;

#define U256(lo, hi) ((u256_t){{ (lo), (hi) }})
#define U384(lo, middle, hi) ((u384_t){{ (lo), (middle), (hi) }})

static inline void wmul_(u128 x, u128 y, u128 *high, u128 *low)
{
    *low = umulti4_(x, y, high);
}

static inline u128 mhi_(u128 x, u128 y)
{
    u128 high;
    (void)umulti4_(x, y, &high);
    return high;
}

/* Keep the partial products 64-bit so wasm32 avoids the generic __multi3.
 * The result is up to two units short of the exact high half, never over. */
static inline u128 mhi_approx_(u128 x, u128 y)
{
    uint64_t xh = x >> 64;
    uint64_t xl = x;
    uint64_t yh = y >> 64;
    uint64_t yl = y;

    return umulditi3_(xh, yh) + (umulditi3_(xh, yl) >> 64)
        + (umulditi3_(xl, yh) >> 64);
}

static inline uint64_t mul_hi_64_(uint64_t x, uint64_t y)
{
    return umulditi3_(x, y) >> 64;
}

static inline u384_t u384_sqr_(u128 x)
{
    u128 high;
    u128 low;
    wmul_(x, x, &high, &low);
    return U384(low, high, 0);
}

static inline u384_t u384_shl_(u384_t x, unsigned shift)
{
    u384_t result = {{ 0, 0, 0 }};
    unsigned word = shift / 128;
    unsigned bits = shift % 128;

    for (unsigned i = 3; i-- > word;) {
        u128 carry = bits && i > word ? x.limb[i - word - 1] >> (128 - bits) : 0;
        result.limb[i] = (x.limb[i - word] << bits) | carry;
    }
    return result;
}

static inline u384_t u384_add_(u384_t a, u384_t b)
{
    u384_t result;
    result.limb[0] = a.limb[0] + b.limb[0];
    u128 carry0 = result.limb[0] < a.limb[0];
    u128 middle = a.limb[1] + b.limb[1];
    u128 carry1 = middle < a.limb[1];
    result.limb[1] = middle + carry0;
    u128 carry2 = result.limb[1] < middle;
    result.limb[2] = a.limb[2] + b.limb[2] + carry1 + carry2;
    return result;
}

static inline u384_t u384_neg_(u384_t x)
{
    u384_t zero = {{ 0, 0, 0 }};
    u384_t result;
    result.limb[0] = zero.limb[0] - x.limb[0];
    u128 borrow0 = x.limb[0] != 0;
    u128 middle = zero.limb[1] - x.limb[1];
    u128 borrow1 = x.limb[1] != 0;
    result.limb[1] = middle - borrow0;
    u128 borrow2 = middle < borrow0;
    result.limb[2] = zero.limb[2] - x.limb[2] - borrow1 - borrow2;
    return result;
}

static inline u384_t u384_sub_(u384_t a, u384_t b)
{
    u384_t result;
    result.limb[0] = a.limb[0] - b.limb[0];
    u128 borrow0 = a.limb[0] < b.limb[0];
    u128 middle = a.limb[1] - b.limb[1];
    u128 borrow1 = a.limb[1] < b.limb[1];
    result.limb[1] = middle - borrow0;
    u128 borrow2 = middle < borrow0;
    result.limb[2] = a.limb[2] - b.limb[2] - borrow1 - borrow2;
    return result;
}

static inline u384_t wmul_128x256_(u128 a, u256_t b)
{
    u128 high;
    u128 low;
    u128 top;
    u128 middle;
    wmul_(a, b.limb[0], &high, &low);
    wmul_(a, b.limb[1], &top, &middle);
    u128 sum = middle + high;
    u128 carry = sum < middle;
    return U384(low, sum, top + carry);
}

static inline u512_t wmul_128x384_(u128 a, u384_t b)
{
    u512_t result;
    u128 high;
    u128 middle_high;
    u128 top;
    u128 middle;
    u128 third;
    wmul_(a, b.limb[0], &high, &result.limb[0]);
    wmul_(a, b.limb[1], &middle_high, &middle);
    wmul_(a, b.limb[2], &top, &third);
    result.limb[1] = middle + high;
    u128 carry = result.limb[1] < middle;
    u128 addend = middle_high + carry;
    result.limb[2] = third + addend;
    u128 high_carry = result.limb[2] < third;
    result.limb[3] = top + high_carry;
    return result;
}

/* High 384 bits of a 384x384 product, up to four low-window units short. */
static inline u384_t u384_mul_hi_(u384_t a, u384_t b)
{
    u128 h02, l02, h11, l11, h20, l20;
    u128 h12, l12, h21, l21, h22, l22;
    wmul_(a.limb[0], b.limb[2], &h02, &l02);
    wmul_(a.limb[1], b.limb[1], &h11, &l11);
    wmul_(a.limb[2], b.limb[0], &h20, &l20);
    wmul_(a.limb[1], b.limb[2], &h12, &l12);
    wmul_(a.limb[2], b.limb[1], &h21, &l21);
    wmul_(a.limb[2], b.limb[2], &h22, &l22);

    u128 sum = l02 + l11;
    u128 carry_low = sum < l02;
    u128 next = sum + l20;
    u128 carry_high = next < sum;
    u128 diagonal = h02 + h11;
    u128 first = diagonal < h02;
    u128 old = diagonal;
    diagonal += h20;
    u128 second = diagonal < old;
    old = diagonal;
    diagonal += carry_low + carry_high;
    u128 third = diagonal < old;
    u128 spill = first + second + third;

    return u384_add_(
        u384_add_(U384(l12, h12, 0), U384(l21, h21, 0)),
        u384_add_(U384(0, l22, h22), U384(diagonal, spill, 0)));
}

static inline u384_t u384_shr_sat_(u384_t x, unsigned shift)
{
    u384_t result = {{ 0, 0, 0 }};
    if (shift >= 384)
        return result;

    unsigned word = shift / 128;
    unsigned bits = shift % 128;
    for (unsigned i = word; i < 3; ++i) {
        u128 carry = bits && i + 1 < 3 ? x.limb[i + 1] << (128 - bits) : 0;
        result.limb[i - word] = (x.limb[i] >> bits) | carry;
    }
    return result;
}

static inline u256_t u256_shr_sat_(u256_t x, unsigned shift)
{
    if (shift >= 256)
        return U256(0, 0);
    if (shift >= 128)
        return U256(x.limb[1] >> (shift - 128), 0);
    if (!shift)
        return x;
    return U256((x.limb[0] >> shift) | (x.limb[1] << (128 - shift)),
        x.limb[1] >> shift);
}

static inline bool u384_any_below_(u384_t x, unsigned n)
{
    if (n >= 384)
        return x.limb[0] || x.limb[1] || x.limb[2];

    unsigned word = n / 128;
    unsigned bits = n % 128;
    for (unsigned i = 0; i < word; ++i)
        if (x.limb[i])
            return true;
    return bits && (x.limb[word] & (~(u128)0 >> (128 - bits)));
}

static inline u256_t u256_add_(u256_t a, u256_t b)
{
    u256_t result;
    result.limb[0] = a.limb[0] + b.limb[0];
    u128 carry = result.limb[0] < a.limb[0];
    result.limb[1] = a.limb[1] + b.limb[1] + carry;
    return result;
}

static inline u256_t u256_sub_(u256_t a, u256_t b)
{
    u256_t result;
    result.limb[0] = a.limb[0] - b.limb[0];
    u128 borrow = a.limb[0] < b.limb[0];
    result.limb[1] = a.limb[1] - b.limb[1] - borrow;
    return result;
}

/* High 256 bits of a 256x256 product, up to two low-window units short. */
static inline u256_t u256_mul_hi_(u256_t a, u256_t b)
{
    u128 high, low, cross_high, cross_low, other_high, other_low;
    wmul_(a.limb[1], b.limb[1], &high, &low);
    wmul_(a.limb[1], b.limb[0], &cross_high, &cross_low);
    wmul_(a.limb[0], b.limb[1], &other_high, &other_low);

    u128 sum = cross_low + other_low;
    u128 carry = sum < cross_low;
    u128 tail = sum + mhi_(a.limb[0], b.limb[0]);
    u128 tail_carry = tail < sum;
    u128 middle = cross_high + other_high;
    u128 overflow = middle < cross_high;
    u128 extra = carry + tail_carry;
    u128 old = middle;
    middle += extra;
    u128 spill = middle < old;
    u128 result_low = low + middle;
    u128 final_carry = result_low < low;
    return U256(result_low, high + overflow + spill + final_carry);
}

static inline u256_t u256_shl_(u256_t x, unsigned shift)
{
    if (shift >= 128)
        return U256(0, x.limb[0] << (shift - 128));
    if (!shift)
        return x;
    return U256(x.limb[0] << shift,
        (x.limb[1] << shift) | (x.limb[0] >> (128 - shift)));
}

static inline int u384_cmp_(u384_t a, u384_t b)
{
    for (unsigned i = 3; i-- > 0;) {
        if (a.limb[i] < b.limb[i])
            return -1;
        if (a.limb[i] > b.limb[i])
            return 1;
    }
    return 0;
}

static inline unsigned u256_clz_(u256_t x)
{
    if (x.limb[1])
        return clzti2_(x.limb[1]);
    return x.limb[0] ? 128 + clzti2_(x.limb[0]) : 256;
}

static inline unsigned u384_clz_(u384_t x)
{
    if (x.limb[2])
        return clzti2_(x.limb[2]);
    if (x.limb[1])
        return 128 + clzti2_(x.limb[1]);
    return x.limb[0] ? 256 + clzti2_(x.limb[0]) : 384;
}

static inline uint64_t funnel_(uint64_t low, uint64_t high, unsigned shift)
{
    return shift ? (high << shift) | (low >> (64 - shift)) : high;
}

static inline uint64_t funnel_down_(uint64_t low, uint64_t high, unsigned shift)
{
    return shift ? (low >> shift) | (high << (64 - shift)) : low;
}

static inline u128 u384_extract_u128_(u384_t x, unsigned shift)
{
    unsigned word = shift / 128;
    unsigned bits = shift % 128;
    u128 high = bits && word + 1 < 3 ? x.limb[word + 1] << (128 - bits) : 0;
    return (x.limb[word] >> bits) | high;
}

#endif

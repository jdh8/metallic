#include "kernel/binary128.h"
#include "kernel/roots.h"
#include "kernel/uint.h"
#include <stdint.h>

/* Correctly rounded binary128 hypot imported from metallic-rs commit
 * 76a9d7f0bce0e2a1dd271b106801325beb866dc3. */

#define HYPOT_GATE_ 32

double sqrt(double);

static u128 abs_diff_(u128 a, u128 b)
{
    return a < b ? b - a : a - b;
}

static int signaling_(u128 bits)
{
    return bits > F128_EXP_MASK && !(bits & F128_QUIET_BIT);
}

static long double subnormal_(u384_t v, u128 c, unsigned t, int exponent)
{
    unsigned shift = -16382 - exponent;
    u128 half = (u128)1 << (shift - 1);
    u128 rest = c & (((u128)1 << shift) - 1);
    u128 n = c >> shift;
    int round = rest > half || (rest == half
        && ((n & 1) || u384_cmp_(u384_shl_(u384_sqr_(c), t), v)));
    return f128_from_bits_(n + round);
}

static long double exact_(u128 ma, u128 mb, unsigned dn, int eb)
{
    u384_t v = u384_add_(u384_shl_(u384_sqr_(ma), 2 * dn), u384_sqr_(mb));
    unsigned length = 384 - u384_clz_(v);
    unsigned h = (length + 1) / 2;
    int exponent = (int)h - 113 + eb;
    unsigned t = 2 * h - 226;
    u128 w = u384_extract_u128_(v, length - 128 + (length & 1));
    u128 seed = (u128)(0x1p158 / sqrt((double)(uint64_t)(w >> 64)));
    u128 q = rsqrt_step_(w, rsqrt_step_(w, seed));
    u128 c = mhi_(w, q) >> 13;

    while (u384_cmp_(u384_shl_(u384_sqr_(c), t), v) > 0)
        --c;
    while (u384_cmp_(u384_shl_(u384_sqr_(c + 1), t), v) <= 0)
        ++c;

    if (exponent < -16382)
        return subnormal_(v, c, t, exponent);
    if (exponent > 16383)
        return f128_from_bits_(F128_EXP_MASK);

    u384_t left = u384_shl_(v, 2);
    u384_t right = u384_shl_(u384_sqr_(2 * c + 1), t);
    int side = u384_cmp_(left, right);
    u128 round = side > 0 || (side == 0 && (c & 1));
    u128 packed = ((u128)(exponent + F128_BIAS) << F128_EXP_SHIFT)
        | (c - F128_IMPLICIT_BIT);
    return f128_from_bits_(packed + round);
}

static u128 hypot_fixed_(u128 ma, u128 mb, unsigned dn, unsigned *carry)
{
    u128 big = ma << 14;
    u128 small = (mb << 14) >> dn;
    u128 high_a;
    u128 low_a;
    u128 high_b;
    u128 low_b;
    wmul_(big, big, &high_a, &low_a);
    wmul_(small, small, &high_b, &low_b);
    u128 low = low_a + low_b;
    u128 v = high_a + high_b + (low < low_a);
    *carry = v >> 126;
    return sqrt_wide_(v << (2 - 2 * *carry));
}

long double hypotl(long double x, long double y)
{
    u128 p = f128_bits_(x) & ~F128_SIGN_MASK;
    u128 q = f128_bits_(y) & ~F128_SIGN_MASK;
    u128 a = p >= q ? p : q;
    u128 b = p >= q ? q : p;

    if (a >= F128_EXP_MASK) {
        if (signaling_(a) || signaling_(b))
            return f128_from_bits_(F128_EXP_MASK | F128_QUIET_BIT);
        if (a == F128_EXP_MASK || b == F128_EXP_MASK)
            return f128_from_bits_(F128_EXP_MASK);
        return f128_from_bits_(a);
    }
    if (!b)
        return f128_from_bits_(a);

    u128 ma;
    u128 mb;
    int ea;
    int eb;
    f128_split_(a, &ma, &ea);
    f128_split_(b, &mb, &eb);
    if (ea - eb > 56)
        return f128_from_bits_(a);
    unsigned dn = ea - eb;
    unsigned carry;
    u128 candidate = hypot_fixed_(ma, mb, dn, &carry);
    int exponent = ea + (int)carry;
    if (exponent > 16383)
        return f128_from_bits_(F128_EXP_MASK);
    if (exponent < -16382 || abs_diff_(candidate & 8191, 4096) <= HYPOT_GATE_)
        return exact_(ma, mb, dn, eb);

    u128 bits = (u128)(F128_BIAS - 1 + exponent) << F128_EXP_SHIFT;
    return f128_from_bits_(bits + ((candidate + 4096) >> 13));
}

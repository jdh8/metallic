/* Correctly rounded binary128 tangent, ported from metallic-rs commit
 * 6c2178df4e987cd67e6541bd32843fd3031cb20a. */
#include "kernel/trig.h"
#include <stdbool.h>

typedef struct {
    u128 fraction;
    int exponent;
} tan_fraction_t;

typedef struct {
    bool valid;
    u128 fraction;
    int exponent;
    u128 flip;
} tan_fast_result_t;

typedef struct {
    u384_t fraction;
    int exponent;
} tan_fraction_wide_t;

static inline u128 tan_flip_(bool negative)
{
    return negative ? F128_SIGN_MASK : 0;
}

static inline tan_fraction_t tan_fraction_(u128 angle, int exponent,
    u128 square, u128 fourth)
{
    u128 a = trig_tan_coef_[0].limb[2] + mhi_approx_(fourth,
        trig_tan_coef_[2].limb[2] + mhi_approx_(fourth,
            trig_tan_coef_[4].limb[2] + mhi_approx_(fourth,
                trig_tan_coef_[6].limb[2])));
    u128 b = trig_tan_coef_[1].limb[2] + mhi_approx_(fourth,
        trig_tan_coef_[3].limb[2] + mhi_approx_(fourth,
            trig_tan_coef_[5].limb[2] + mhi_approx_(fourth,
                trig_tan_coef_[7].limb[2])));
    u128 polynomial = a + mhi_approx_(square, b);
    u128 addend = mhi_approx_(mhi_approx_(angle, square), polynomial);
    u128 sum = angle + addend;
    unsigned carry = sum < angle;
    return (tan_fraction_t) {
        .fraction = (sum >> carry) | (u128)carry << 127,
        .exponent = exponent + (int)carry,
    };
}

static inline tan_fraction_t tan_quotient_(u128 numerator, u128 denominator)
{
    u128 reciprocal = atan_recip_128_(denominator);
    u128 high;
    u128 low;
    wmul_(numerator, reciprocal, &high, &low);
    u128 quotient0 = high << 1 | low >> 127;

    u128 product_high;
    u128 product_low;
    wmul_(denominator, quotient0, &product_high, &product_low);
    u256_t remainder = u256_sub_(U256(numerator << 127, numerator >> 1),
        U256(product_low, product_high));
    u128 correction_high;
    u128 correction_low;
    wmul_((remainder.limb[0] >> 4) | (remainder.limb[1] << 124),
        reciprocal, &correction_high, &correction_low);
    u256_t quotient = u256_add_(U256(0, quotient0),
        U256((correction_low >> 122) | (correction_high << 6),
            correction_high >> 122));
    unsigned leading = (unsigned)clzti2_(quotient.limb[1]);
    return (tan_fraction_t) {
        .fraction = trig_top_256_(quotient, leading),
        .exponent = 1 - (int)leading,
    };
}

static inline tan_fast_result_t tan_fast_(u128 mantissa, int exponent)
{
    if (exponent < TRIG_DIRECT_) {
        u128 angle = mantissa << 15;
        int angle_exponent = exponent + 1;
        trig_squares_t powers = trig_squares_(angle, angle_exponent);
        tan_fraction_t value = tan_fraction_(angle, angle_exponent,
            powers.square, powers.fourth);
        return (tan_fast_result_t) {
            true, value.fraction, value.exponent, 0
        };
    }

    trig_residual_t reduction;
    if (!trig_reduce_(mantissa, exponent, &reduction))
        return (tan_fast_result_t) { .valid = false };
    trig_squares_t powers = trig_squares_(reduction.fraction,
        reduction.exponent);
    tan_fraction_t tangent = tan_fraction_(reduction.fraction,
        reduction.exponent, powers.square, powers.fourth);
    bool odd = reduction.n & 128;
    unsigned index = reduction.n & 127;

    if (!index) {
        if (!odd)
            return (tan_fast_result_t) { true, tangent.fraction,
                tangent.exponent, tan_flip_(reduction.negative) };
        tan_fraction_t reciprocal = tan_quotient_((u128)1 << 127,
            tangent.fraction);
        return (tan_fast_result_t) { true, reciprocal.fraction,
            reciprocal.exponent + 1 - tangent.exponent,
            tan_flip_(!reduction.negative) };
    }

    u256_t breakpoint = U256(trig_tan_[index].limb[1],
        trig_tan_[index].limb[2]);
    u256_t numerator = trig_add_signed_256_(breakpoint,
        trig_place_256_(tangent.fraction,
            (unsigned)(tangent.exponent + 121)), reduction.negative);
    u128 product_high;
    u128 product_low;
    wmul_(trig_tan_[index].limb[2], tangent.fraction,
        &product_high, &product_low);
    u256_t product = u256_shr_sat_(U256(product_low, product_high),
        (unsigned)-tangent.exponent);
    u256_t denominator = trig_add_signed_256_(
        U256(0, (u128)1 << 121), product, !reduction.negative);

    u128 mask = 0 - (u128)odd;
    u128 swap_low = (numerator.limb[0] ^ denominator.limb[0]) & mask;
    u128 swap_high = (numerator.limb[1] ^ denominator.limb[1]) & mask;
    u256_t n = U256(numerator.limb[0] ^ swap_low,
        numerator.limb[1] ^ swap_high);
    u256_t d = U256(denominator.limb[0] ^ swap_low,
        denominator.limb[1] ^ swap_high);
    unsigned numerator_leading = (unsigned)clzti2_(n.limb[1]);
    unsigned denominator_leading = (unsigned)clzti2_(d.limb[1]);
    tan_fraction_t quotient = tan_quotient_(
        trig_top_256_(n, numerator_leading),
        trig_top_256_(d, denominator_leading));

    return (tan_fast_result_t) {
        true,
        quotient.fraction,
        quotient.exponent + (int)denominator_leading - (int)numerator_leading,
        tan_flip_(odd),
    };
}

static tan_fraction_wide_t tan_fraction_384_(u384_t angle,
    int exponent, u384_t square)
{
    u384_t polynomial = trig_tan_coef_[24];
    for (unsigned i = 24; i-- > 0;)
        polynomial = u384_add_(trig_tan_coef_[i],
            u384_mul_hi_(square, polynomial));
    u384_t half = u384_shr_sat_(angle, 1);
    u384_t value = u384_add_(half,
        u384_mul_hi_(half, u384_mul_hi_(square, polynomial)));
    unsigned leading = u384_clz_(value);
    return (tan_fraction_wide_t) {
        .fraction = u384_shl_(value, leading),
        .exponent = exponent + 1 - (int)leading,
    };
}

static u384_t tan_refine_(u384_t numerator, u384_t denominator,
    u384_t value, u128 reciprocal, u128 margin)
{
    u384_t remainder = u384_sub_(u384_sub_(
        u384_shr_sat_(numerator, 2),
        u384_mul_hi_(denominator, value)), U384(margin, 0, 0));
    u512_t correction = wmul_128x384_(reciprocal, remainder);
    return u384_add_(value, U384(
        (correction.limb[0] >> 126) | (correction.limb[1] << 2),
        (correction.limb[1] >> 126) | (correction.limb[2] << 2),
        (correction.limb[2] >> 126) | (correction.limb[3] << 2)));
}

static tan_fraction_wide_t tan_quotient_384_(u384_t numerator,
    u384_t denominator)
{
    u128 reciprocal = atan_recip_128_(denominator.limb[2]) - 4;
    u512_t product = wmul_128x384_(reciprocal, numerator);
    u384_t value = U384(product.limb[1], product.limb[2], product.limb[3]);
    value = tan_refine_(numerator, denominator, value, reciprocal, 8);
    value = tan_refine_(numerator, denominator, value, reciprocal, 0);
    unsigned leading = u384_clz_(value);
    return (tan_fraction_wide_t) {
        .fraction = u384_shl_(value, leading),
        .exponent = 2 - (int)leading,
    };
}

static __attribute__((cold, noinline)) long double
tan_accurate_(u128 mantissa, int exponent, u128 sign)
{
    trig_residual_wide_t reduction = exponent < TRIG_DIRECT_
        ? (trig_residual_wide_t) {
            .n = 0,
            .negative = false,
            .fraction = U384(0, 0, mantissa << 15),
            .exponent = exponent + 1,
        }
        : trig_reduce_wide_(mantissa, exponent);
    u384_t square = u384_shr_sat_(u384_mul_hi_(reduction.fraction,
        reduction.fraction), (unsigned)(-2 * reduction.exponent));
    tan_fraction_wide_t tangent = tan_fraction_384_(reduction.fraction,
        reduction.exponent, square);
    bool odd = reduction.n & 128;
    unsigned index = reduction.n & 127;

    if (!index) {
        if (!odd)
            return __metallic_atan_round_384(tangent.fraction, tangent.exponent,
                sign ^ tan_flip_(reduction.negative));
        tan_fraction_wide_t reciprocal = tan_quotient_384_(
            U384(0, 0, (u128)1 << 127), tangent.fraction);
        return __metallic_atan_round_384(reciprocal.fraction,
            reciprocal.exponent + 1 - tangent.exponent,
            sign ^ tan_flip_(!reduction.negative));
    }

    u384_t angle = u384_shr_sat_(tangent.fraction,
        (unsigned)(7 - tangent.exponent));
    u384_t product = u384_shr_sat_(u384_mul_hi_(trig_tan_[index],
        tangent.fraction), (unsigned)-tangent.exponent);
    u384_t one = U384(0, 0, (u128)1 << 121);
    u384_t numerator = reduction.negative
        ? u384_sub_(trig_tan_[index], angle)
        : u384_add_(trig_tan_[index], angle);
    u384_t denominator = reduction.negative
        ? u384_add_(one, product) : u384_sub_(one, product);
    if (odd) {
        u384_t swap = numerator;
        numerator = denominator;
        denominator = swap;
    }
    unsigned numerator_leading = u384_clz_(numerator);
    unsigned denominator_leading = u384_clz_(denominator);
    tan_fraction_wide_t quotient = tan_quotient_384_(
        u384_shl_(numerator, numerator_leading),
        u384_shl_(denominator, denominator_leading));
    return __metallic_atan_round_384(quotient.fraction,
        quotient.exponent + (int)denominator_leading - (int)numerator_leading,
        sign ^ tan_flip_(odd));
}

long double tanl(long double x)
{
    u128 bits = f128_bits_(x);
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (magnitude >= F128_EXP_MASK)
        return trig_edge_(bits, magnitude);
    if (magnitude < TRIG_TINY_)
        return x;

    u128 mantissa;
    int exponent;
    f128_split_(magnitude, &mantissa, &exponent);
    tan_fast_result_t fast = tan_fast_(mantissa, exponent);
    u128 sign = bits & F128_SIGN_MASK;
    long double result;
    if (fast.valid && trig_round_fast_(fast.fraction, fast.exponent,
            sign ^ fast.flip, &result))
        return result;
    return tan_accurate_(mantissa, exponent, sign);
}

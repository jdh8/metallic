/* Correctly rounded binary128 sine and cosine, ported from metallic-rs
 * commit 6c2178df4e987cd67e6541bd32843fd3031cb20a. */
#include "kernel/trig.h"
#include <stdbool.h>

typedef struct {
    bool valid;
    u128 fraction;
    int exponent;
    u128 flip;
} trig_fast_result_t;

static inline u128 trig_sin_fraction_(u128 angle, u128 square, u128 fourth)
{
    u128 a = trig_sin_coef_[0].limb[2] + mhi_approx_(fourth,
        trig_sin_coef_[2].limb[2] + mhi_approx_(fourth,
            trig_sin_coef_[4].limb[2]));
    u128 b = trig_sin_coef_[1].limb[2] + mhi_approx_(fourth,
        trig_sin_coef_[3].limb[2] + mhi_approx_(fourth,
            trig_sin_coef_[5].limb[2]));
    return angle - mhi_approx_(mhi_approx_(angle, square),
        a - mhi_approx_(square, b));
}

static inline u128 trig_cos_correction_(u128 unshifted,
    u128 square, u128 fourth)
{
    u128 even = trig_cos_coef_[1].limb[2] + mhi_approx_(fourth,
        trig_cos_coef_[3].limb[2] + mhi_approx_(fourth,
            trig_cos_coef_[5].limb[2]));
    u128 odd = trig_cos_coef_[2].limb[2] + mhi_approx_(fourth,
        trig_cos_coef_[4].limb[2] + mhi_approx_(fourth,
            trig_cos_coef_[6].limb[2]));
    return mhi_approx_(unshifted, trig_cos_coef_[0].limb[2]
        - mhi_approx_(square, even - mhi_approx_(square, odd)));
}

static inline trig_fast_result_t trig_fast_(u128 mantissa, int exponent,
    bool cosine)
{
    if (exponent < TRIG_DIRECT_) {
        u128 angle = mantissa << 15;
        int angle_exponent = exponent + 1;
        trig_squares_t powers = trig_squares_(angle, angle_exponent);

        if (cosine) {
            u128 correction = trig_cos_correction_(powers.unshifted,
                powers.square, powers.fourth);
            u256_t placed = trig_place_256_(correction,
                (unsigned)(2 * angle_exponent + 128));
            u256_t value = u256_sub_(U256(~(u128)0, ~(u128)0), placed);
            return (trig_fast_result_t) { true, value.limb[1], 0, 0 };
        }

        u128 value = trig_sin_fraction_(angle, powers.square, powers.fourth);
        unsigned leading = (unsigned)clzti2_(value);
        return (trig_fast_result_t) {
            true, value << leading, angle_exponent - (int)leading, 0
        };
    }

    trig_residual_t reduction;
    if (!trig_reduce_(mantissa, exponent, &reduction))
        return (trig_fast_result_t) { .valid = false };

    trig_squares_t powers = trig_squares_(reduction.fraction,
        reduction.exponent);
    u128 sine = trig_sin_fraction_(reduction.fraction,
        powers.square, powers.fourth);
    unsigned quadrant = (reduction.n >> 7) + cosine;
    bool want_cosine = quadrant & 1;
    u128 flip = quadrant & 2 ? F128_SIGN_MASK : 0;
    unsigned index = reduction.n & 127;

    if (!index && !want_cosine) {
        unsigned leading = (unsigned)clzti2_(sine);
        if (reduction.negative)
            flip ^= F128_SIGN_MASK;
        return (trig_fast_result_t) {
            true, sine << leading, reduction.exponent - (int)leading, flip
        };
    }

    u128 cosine_correction = trig_cos_correction_(powers.unshifted,
        powers.square, powers.fourth);
    const u384_t *first = &trig_sincos_[index][want_cosine];
    const u384_t *second = &trig_sincos_[index][!want_cosine];
    u256_t base = U256(first->limb[1], first->limb[2]);
    u256_t cosine_term = trig_place_256_(
        mhi_approx_(first->limb[2], cosine_correction),
        (unsigned)(2 * reduction.exponent + 128));
    u256_t sine_term = trig_place_256_(
        mhi_approx_(second->limb[2], sine),
        (unsigned)(reduction.exponent + 128));
    u256_t value = trig_add_signed_256_(u256_sub_(base, cosine_term),
        sine_term, reduction.negative != want_cosine);
    unsigned leading = (unsigned)clzti2_(value.limb[1]);

    return (trig_fast_result_t) {
        true, trig_top_256_(value, leading), -(int)leading, flip
    };
}

static u384_t trig_alternating_(u384_t square,
    const u384_t *coefficient, unsigned count)
{
    u384_t value = coefficient[count - 1];
    for (unsigned i = count - 1; i-- > 0;)
        value = u384_sub_(coefficient[i], u384_mul_hi_(square, value));
    return value;
}

static __attribute__((cold, noinline)) long double
trig_accurate_(u128 mantissa, int exponent, bool cosine, u128 sign)
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
    unsigned quadrant = (reduction.n >> 7) + cosine;
    bool want_cosine = quadrant & 1;
    if (quadrant & 2)
        sign ^= F128_SIGN_MASK;
    unsigned index = reduction.n & 127;

    u384_t sine = u384_sub_(reduction.fraction,
        u384_mul_hi_(reduction.fraction, u384_mul_hi_(square,
            trig_alternating_(square, trig_sin_coef_, 18))));
    unsigned sine_leading = u384_clz_(sine);
    sine = u384_shl_(sine, sine_leading);
    int sine_exponent = reduction.exponent - (int)sine_leading;

    if (!index && !want_cosine)
        return __metallic_atan_round_384(sine, sine_exponent,
            sign ^ (reduction.negative ? F128_SIGN_MASK : 0));

    u384_t cosine_correction = u384_mul_hi_(square,
        trig_alternating_(square, trig_cos_coef_, 18));
    u384_t first = trig_sincos_[index][want_cosine];
    u384_t second = trig_sincos_[index][!want_cosine];
    u384_t base = u384_sub_(first,
        u384_mul_hi_(first, cosine_correction));
    u384_t term = u384_shr_sat_(u384_mul_hi_(second, sine),
        (unsigned)-sine_exponent);
    u384_t value = reduction.negative != want_cosine
        ? u384_sub_(base, term) : u384_add_(base, term);
    unsigned leading = u384_clz_(value);
    return __metallic_atan_round_384(u384_shl_(value, leading),
        -(int)leading, sign);
}

static long double trig_dispatch_(long double x, bool cosine)
{
    u128 bits = f128_bits_(x);
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (magnitude >= F128_EXP_MASK)
        return trig_edge_(bits, magnitude);
    if (magnitude < TRIG_TINY_)
        return cosine ? 1.0L : x;

    u128 sign = cosine ? 0 : bits & F128_SIGN_MASK;
    u128 mantissa;
    int exponent;
    f128_split_(magnitude, &mantissa, &exponent);
    trig_fast_result_t fast = trig_fast_(mantissa, exponent, cosine);
    long double result;
    if (fast.valid && trig_round_fast_(fast.fraction, fast.exponent,
            sign ^ fast.flip, &result))
        return result;
    return trig_accurate_(mantissa, exponent, cosine, sign);
}

long double sinl(long double x) { return trig_dispatch_(x, false); }
long double cosl(long double x) { return trig_dispatch_(x, true); }

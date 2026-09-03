/* Correctly rounded binary128 atan/atan2.
 * Ported from metallic-rs 6c2178df4e987cd67e6541bd32843fd3031cb20a. */
#include "kernel/atan.h"
#include "kernel/atan2tab.h"
#include <stdbool.h>
#include <stdint.h>

static const u128 ATAN2_PI = U128(0x4000921fb54442d1, 0x8469898cc51701b8);
static const u128 ATAN2_PI_2 = U128(0x3fff921fb54442d1, 0x8469898cc51701b8);
static const u128 ATAN2_PI_4 = U128(0x3ffe921fb54442d1, 0x8469898cc51701b8);

typedef atan_reduction_t atan2_reduction_t;
typedef atan_frac128_t atan2_frac128_t;

typedef struct {
    u384_t fraction;
    int exponent;
} atan2_frac384_t;

static inline bool atan2_relative_(const atan2_reduction_t *r)
{
    return r->sector == 0 && r->quadrant == 0;
}

static inline double atan2_double_from_bits_(uint64_t bits)
{
    union {
        double value;
        uint64_t bits;
    } value = { .bits = bits };
    return value.value;
}

static __attribute__((cold, noinline)) long double atan2_edge_(u128 ybits, u128 xbits)
{
    u128 ay = ybits & ~F128_SIGN_MASK;
    u128 ax = xbits & ~F128_SIGN_MASK;

    if (ay > F128_EXP_MASK)
        return f128_from_bits_(ybits | F128_QUIET_BIT);
    if (ax > F128_EXP_MASK)
        return f128_from_bits_(xbits | F128_QUIET_BIT);

    u128 sign = ybits & F128_SIGN_MASK;
    bool xneg = (xbits >> 127) != 0;

    if (ay == F128_EXP_MASK) {
        u128 magnitude = ax != F128_EXP_MASK ? ATAN2_PI_2
            : xneg ? ATAN2_FRAC_3PI_4 : ATAN2_PI_4;
        return f128_from_bits_(sign | magnitude);
    }
    if (ay == 0)
        return f128_from_bits_(sign | (xneg ? ATAN2_PI : 0));
    if (ax == 0)
        return f128_from_bits_(sign | ATAN2_PI_2);
    return f128_from_bits_(sign | (xneg ? ATAN2_PI : 0));
}

static atan2_reduction_t atan2_reduce_(u128 ay, u128 ax, bool xneg)
{
    bool swap = ay > ax;
    u128 big = swap ? ay : ax;
    u128 small = swap ? ax : ay;
    u128 d;
    u128 n;
    int ex;
    int ey;
    f128_split_(big, &d, &ex);
    f128_split_(small, &n, &ey);
    unsigned dn = (unsigned)(ex - ey);

    unsigned sector = 0;
    if (dn < 8) {
        double scale = atan2_double_from_bits_((uint64_t)(1029 - dn) << 52);
        sector = (unsigned)(scale * (double)(uint32_t)(n >> 98)
            / (double)(uint32_t)(d >> 98) + 0.5);
    }

    u128 numerator;
    u128 denominator;
    bool negative;
    int reduction_scale;
    if (sector == 0) {
        numerator = n << 15;
        denominator = d << 15;
        negative = false;
        reduction_scale = -(int)dn;
    } else {
        __int128 far = (__int128)(d * sector) << dn;
        __int128 signed_numerator = (__int128)(n << 6) - far;
        u128 magnitude = signed_numerator < 0
            ? (u128)(-signed_numerator) : (u128)signed_numerator;
        unsigned lzn = magnitude ? (unsigned)clzti2_(magnitude) : 128;
        u128 kd = (d << (6 + dn)) + n * sector;
        unsigned lzd = (unsigned)clzti2_(kd);
        numerator = magnitude ? magnitude << lzn : 0;
        denominator = kd << lzd;
        negative = signed_numerator < 0;
        reduction_scale = (int)lzd - (int)lzn;
    }

    return (atan2_reduction_t) {
        .numerator = numerator,
        .denominator = denominator,
        .scale = reduction_scale,
        .negative = negative,
        .sector = sector,
        .quadrant = swap ? 1 : 2 * (unsigned)xneg,
        .negate = swap != xneg,
    };
}

static u256_t atan2_recip_256_(u128 d)
{
    u128 r = atan_recip_128_(d);
    u128 high;
    u128 low;
    wmul_(d, r, &high, &low);
    u256_t residual = u256_sub_(U256(0, (u128)1 << 126), U256(low, high));
    u384_t c = wmul_128x256_(r, residual);
    return u256_add_(U256(0, r), U256(
        (c.limb[0] >> 126) | (c.limb[1] << 2),
        (c.limb[1] >> 126) | (c.limb[2] << 2)));
}

static u384_t atan2_recip_384_(u128 d)
{
    u256_t r = atan2_recip_256_(d);
    u384_t p = wmul_128x256_(d, r);
    u384_t residual = u384_sub_(U384(0, 0, (u128)1 << 126), p);
    u256_t c = u256_shl_(u256_mul_hi_(r,
        U256(residual.limb[0], residual.limb[1])), 2);
    return u384_add_(U384(0, r.limb[0], r.limb[1]),
        U384(c.limb[0], c.limb[1], 0));
}

static inline u128 atan2_top_256_(u256_t value, unsigned shift)
{
    uint64_t low = value.limb[0] >> 64;
    uint64_t middle = value.limb[1];
    uint64_t high = value.limb[1] >> 64;
    return (u128)funnel_(low, middle, shift)
        | (u128)funnel_(middle, high, shift) << 64;
}

static atan2_frac128_t atan2_quotient_128_(const atan2_reduction_t *r)
{
    u128 high;
    u128 low;
    wmul_(r->numerator, atan_recip_128_(r->denominator), &high, &low);
    unsigned lz = (unsigned)clzti2_(high);
    return (atan2_frac128_t) {
        .fraction = atan2_top_256_(U256(low, high), lz),
        .exponent = r->scale + 2 - (int)lz,
    };
}

static atan2_frac384_t atan2_quotient_384_(const atan2_reduction_t *r)
{
    u512_t p = wmul_128x384_(r->numerator, atan2_recip_384_(r->denominator));
    unsigned lz = (unsigned)clzti2_(p.limb[3]);
    u384_t fraction;
    if (lz == 0) {
        fraction = U384(p.limb[1], p.limb[2], p.limb[3]);
    } else {
        fraction = U384(
            (p.limb[1] << lz) | (p.limb[0] >> (128 - lz)),
            (p.limb[2] << lz) | (p.limb[1] >> (128 - lz)),
            (p.limb[3] << lz) | (p.limb[2] >> (128 - lz)));
    }
    return (atan2_frac384_t) {
        .fraction = fraction,
        .exponent = r->scale + 2 - (int)lz,
    };
}

static inline u128 atan2_shr_round_(u128 x, unsigned shift)
{
    unsigned bits = shift & 63;
    uint64_t low;
    uint64_t high;
    if (shift < 64) {
        low = x;
        high = x >> 64;
    } else {
        low = x >> 64;
        high = 0;
    }
    unsigned below = shift - 1;
    uint64_t word = below < 64 ? (uint64_t)x : (uint64_t)(x >> 64);
    uint64_t bit = (word >> (below & 63)) & 1;
    return (u128)funnel_down_(low, high, bits) | (u128)(high >> bits) << 64 | bit;
}

static u128 atan2_fraction_(u128 t, int exponent)
{
    unsigned shift = (unsigned)(-2 * exponent);
    if (shift >= 128)
        return t;

    u128 u = atan2_shr_round_(mhi_approx_(t, t), shift);
    u128 cube = mhi_approx_(t, u);
    u128 v = mhi_approx_(u, u);
    uint64_t vh = v >> 64;
    uint64_t tail_even = (uint64_t)(ATAN2_COEF[4][2] >> 64)
        + mul_hi_64_(vh, (uint64_t)(ATAN2_COEF[6][2] >> 64)
            + mul_hi_64_(vh, ATAN2_COEF[8][2] >> 64));
    uint64_t tail_odd = (uint64_t)(ATAN2_COEF[5][2] >> 64)
        + mul_hi_64_(vh, (uint64_t)(ATAN2_COEF[7][2] >> 64)
            + mul_hi_64_(vh, ATAN2_COEF[9][2] >> 64));
    u128 even = ATAN2_COEF[0][2] + mhi_approx_(v,
        ATAN2_COEF[2][2] + mhi_approx_(v, (u128)tail_even << 64));
    u128 odd = ATAN2_COEF[1][2] + mhi_approx_(v,
        ATAN2_COEF[3][2] + mhi_approx_(v, (u128)tail_odd << 64));
    return t - mhi_approx_(cube, even - mhi_approx_(u, odd));
}

static inline u256_t atan2_place_256_(u128 fraction, unsigned shift)
{
    unsigned bits = shift & 63;
    uint64_t a;
    uint64_t b;
    uint64_t c;
    if (shift < 64) {
        a = fraction;
        b = fraction >> 64;
        c = 0;
    } else {
        a = 0;
        b = fraction;
        c = fraction >> 64;
    }
    return U256((u128)(a << bits) | (u128)funnel_(a, b, bits) << 64,
        (u128)funnel_(b, c, bits) | (u128)funnel_(c, 0, bits) << 64);
}

static inline u256_t atan2_add_signed_256_(u256_t a, u256_t b, bool negative)
{
    u128 mask = (u128)0 - (u128)negative;
    return u256_add_(a, u256_add_(
        U256(b.limb[0] ^ mask, b.limb[1] ^ mask), U256(negative, 0)));
}

static atan2_frac128_t atan2_fast_(const atan2_reduction_t *r)
{
    if (r->numerator == 0)
        return __metallic_atan_combine(U256(0, 0), r->sector, r->negative,
            r->quadrant, r->negate);

    atan2_frac128_t quotient = atan2_quotient_128_(r);
    u128 fraction = atan2_fraction_(quotient.fraction, quotient.exponent);
    if (atan2_relative_(r)) {
        unsigned lz = (unsigned)clzti2_(fraction);
        return (atan2_frac128_t) {
            .fraction = fraction << lz,
            .exponent = quotient.exponent - (int)lz,
        };
    }
    return __metallic_atan_combine(__metallic_atan_place(fraction,
        quotient.exponent), r->sector,
        r->negative, r->quadrant, r->negate);
}

static bool atan2_round_fast_(u128 fraction, int exponent, u128 sign, long double *result)
{
    /* C's LDBL_MIN_EXP is -16381: e2 == -16382 is already subnormal. */
    if (exponent < -16381)
        return false;
    u128 rest = fraction & ATAN_GUARD_MASK_;
    u128 distance = rest > ATAN_GUARD_HALF_
        ? rest - ATAN_GUARD_HALF_ : ATAN_GUARD_HALF_ - rest;
    if (distance <= ATAN_ZIV_GATE)
        return false;

    u128 bits = sign | ((u128)(exponent + 16382) << F128_EXP_SHIFT)
        + (fraction >> ATAN_GUARD_) - F128_IMPLICIT_BIT
        + (rest > ATAN_GUARD_HALF_);
    *result = f128_from_bits_(bits);
    return true;
}

static u384_t atan2_correction_384_(u384_t t, int exponent)
{
    unsigned shift = (unsigned)(-2 * exponent);
    if (shift >= 384)
        return U384(0, 0, 0);

    u384_t u = u384_shr_sat_(u384_mul_hi_(t, t), shift);
    u128 narrow = ATAN2_COEF[17][2];
    for (unsigned i = 17; i-- > 11;)
        narrow = ATAN2_COEF[i][2] - mhi_(u.limb[2], narrow);

    u256_t middle = U256(0, narrow);
    for (unsigned i = 11; i-- > 2;)
        middle = u256_sub_(U256(ATAN2_COEF[i][1], ATAN2_COEF[i][2]),
            u256_mul_hi_(U256(u.limb[1], u.limb[2]), middle));

    u384_t q = U384(0, middle.limb[0], middle.limb[1]);
    for (unsigned i = 2; i-- > 0;)
        q = u384_sub_(U384(ATAN2_COEF[i][0], ATAN2_COEF[i][1],
            ATAN2_COEF[i][2]), u384_mul_hi_(u, q));
    return u384_mul_hi_(u, q);
}

static inline u384_t atan2_fraction_384_(u384_t t, int exponent)
{
    return u384_sub_(t, u384_mul_hi_(t, atan2_correction_384_(t, exponent)));
}

static long double atan2_round_384_(u384_t fraction, int exponent, u128 sign)
{
    int keep = exponent + 16494;
    if (keep > 113)
        keep = 113;
    if (keep < 0)
        return f128_from_bits_(sign);
    if (keep == 0) {
        u384_t halfway = U384(0, 0, (u128)1 << 127);
        return f128_from_bits_(sign | (u384_cmp_(fraction, halfway) != 0));
    }

    u128 biased = keep == 113 ? (u128)(exponent + 16382) : 0;
    u128 implicit = keep == 113 ? F128_IMPLICIT_BIT : 0;
    unsigned shift = (unsigned)(384 - keep);
    u128 mantissa = u384_extract_u128_(fraction, shift);
    u128 round_bit = fraction.limb[(shift - 1) / 128]
        >> ((shift - 1) % 128) & 1;
    bool up = round_bit && (u384_any_below_(fraction, shift - 1) || (mantissa & 1));
    return f128_from_bits_(sign | (biased << F128_EXP_SHIFT)
        + mantissa - implicit + up);
}

u128 __metallic_atan_shr_round(u128 x, unsigned shift)
{
    return atan2_shr_round_(x, shift);
}

u256_t __metallic_atan_place(u128 fraction, int exponent)
{
    int position = exponent + 125;
    if (position >= 256)
        return U256(0, 0);
    if (position >= 128)
        return U256(0, fraction << (unsigned)(position - 128));
    if (position >= 0)
        return atan2_place_256_(fraction, (unsigned)position);
    if (position > -128)
        return U256(fraction >> (unsigned)-position, 0);
    return U256(0, 0);
}

atan_frac128_t __metallic_atan_combine(u256_t theta, unsigned sector, bool negative,
    unsigned quadrant, bool negate)
{
    u256_t phi = U256(ATAN2_PHI[sector][1], ATAN2_PHI[sector][2]);
    u256_t arc = atan2_add_signed_256_(phi, theta, negative);
    u256_t offset = U256(ATAN2_QOFF[quadrant][1], ATAN2_QOFF[quadrant][2]);
    u256_t result = atan2_add_signed_256_(offset, arc, negate);
    unsigned lz = (unsigned)clzti2_(result.limb[1]);
    return (atan_frac128_t) {
        .fraction = atan2_top_256_(result, lz),
        .exponent = 3 - (int)lz,
    };
}

atan_frac128_t __metallic_atan_fast(const atan_reduction_t *r)
{
    return atan2_fast_(r);
}

bool __metallic_atan_round_fast(u128 fraction, int exponent, u128 sign,
    long double *result)
{
    return atan2_round_fast_(fraction, exponent, sign, result);
}

u384_t __metallic_atan_fraction_384(u384_t t, int exponent)
{
    return atan2_fraction_384_(t, exponent);
}

long double __metallic_atan_assemble_384(u384_t theta, bool negative, unsigned sector,
    unsigned quadrant, bool negate, u128 sign)
{
    u384_t phi = U384(ATAN2_PHI[sector][0], ATAN2_PHI[sector][1],
        ATAN2_PHI[sector][2]);
    u384_t arc = negative ? u384_sub_(phi, theta) : u384_add_(phi, theta);
    u384_t offset = U384(ATAN2_QOFF[quadrant][0],
        ATAN2_QOFF[quadrant][1], ATAN2_QOFF[quadrant][2]);
    u384_t result = negate ? u384_sub_(offset, arc) : u384_add_(offset, arc);
    unsigned lz = (unsigned)clzti2_(result.limb[2]);
    return atan2_round_384_(u384_shl_(result, lz), 3 - (int)lz, sign);
}

long double __metallic_atan_round_384(u384_t fraction, int exponent, u128 sign)
{
    return atan2_round_384_(fraction, exponent, sign);
}

static __attribute__((cold, noinline)) long double
atan2_accurate_(const atan2_reduction_t *r, u128 sign)
{
    if (atan2_relative_(r)) {
        atan2_frac384_t quotient = atan2_quotient_384_(r);
        u384_t fraction = atan2_fraction_384_(quotient.fraction, quotient.exponent);
        unsigned lz = (unsigned)clzti2_(fraction.limb[2]);
        return atan2_round_384_(u384_shl_(fraction, lz),
            quotient.exponent - (int)lz, sign);
    }

    u384_t theta = U384(0, 0, 0);
    if (r->numerator) {
        atan2_frac384_t quotient = atan2_quotient_384_(r);
        theta = u384_shr_sat_(atan2_fraction_384_(quotient.fraction,
            quotient.exponent), (unsigned)(3 - quotient.exponent));
    }

    return __metallic_atan_assemble_384(theta, r->negative, r->sector, r->quadrant,
        r->negate, sign);
}

long double atan2l(long double y, long double x)
{
    u128 ybits = f128_bits_(y);
    u128 xbits = f128_bits_(x);
    u128 ay = ybits & ~F128_SIGN_MASK;
    u128 ax = xbits & ~F128_SIGN_MASK;

    if (ay - 1 >= F128_EXP_MASK - 1 || ax - 1 >= F128_EXP_MASK - 1)
        return atan2_edge_(ybits, xbits);

    u128 sign = ybits & F128_SIGN_MASK;
    atan2_reduction_t reduction = atan2_reduce_(ay, ax, (xbits >> 127) != 0);
    atan2_frac128_t fast = atan2_fast_(&reduction);
    long double result;
    if (atan2_round_fast_(fast.fraction, fast.exponent, sign, &result))
        return result;
    return atan2_accurate_(&reduction, sign);
}

typedef struct {
    atan_reduction_t reduction;
    u128 exact_fraction;
    int exact_exponent;
    bool exact;
} atan_input_t;

static atan_reduction_t atan_normalize_(__int128 kn, u128 kd,
    unsigned sector, unsigned quadrant, bool negate)
{
    bool negative = kn < 0;
    u128 magnitude = negative ? (u128)-kn : (u128)kn;
    unsigned lzn = magnitude ? (unsigned)clzti2_(magnitude) : 128;
    unsigned lzd = (unsigned)clzti2_(kd);
    return (atan_reduction_t) {
        .numerator = magnitude ? magnitude << lzn : 0,
        .denominator = kd << lzd,
        .scale = (int)lzd - (int)lzn,
        .negative = negative,
        .sector = sector,
        .quadrant = quadrant,
        .negate = negate,
    };
}

static atan_input_t atan_reduce_(u128 ax)
{
    static const u128 one = (u128)F128_BIAS << F128_EXP_SHIFT;
    u128 m;
    int e;
    f128_split_(ax, &m, &e);

    if (ax < one) {
        unsigned dn = (unsigned)-e;
        unsigned i = dn < 8
            ? (unsigned)(((m >> (105 + dn)) + 1) >> 1) : 0;
        if (i == 0)
            return (atan_input_t) {
                .exact_fraction = m << 15,
                .exact_exponent = 1 - (int)dn,
                .exact = true,
            };
        __int128 kn = (__int128)(m << 6)
            - ((__int128)i << (112 + dn));
        u128 kd = ((u128)1 << (118 + dn)) + m * i;
        return (atan_input_t) {
            .reduction = atan_normalize_(kn, kd, i, 0, false),
        };
    }

    unsigned dn = (unsigned)e;
    unsigned i = 0;
    if (dn < 8) {
        uint64_t q = ((uint64_t)1 << 63) / (uint64_t)(m >> (64 - dn));
        i = (unsigned)((q + 256) >> 9);
    }
    if (i == 0)
        return (atan_input_t) {
            .reduction = {
                .numerator = (u128)1 << 127,
                .denominator = m << 15,
                .scale = -(int)dn,
                .sector = 0,
                .quadrant = 1,
                .negate = true,
            },
        };
    __int128 kn = ((__int128)1 << 118) - ((__int128)(m * i) << dn);
    u128 kd = (m << (6 + dn)) + ((u128)i << 112);
    return (atan_input_t) {
        .reduction = atan_normalize_(kn, kd, i, 1, true),
    };
}

static atan_frac128_t atan_input_fast_(const atan_input_t *input)
{
    if (!input->exact)
        return atan2_fast_(&input->reduction);
    u128 fraction = atan2_fraction_(input->exact_fraction,
        input->exact_exponent);
    unsigned lz = (unsigned)clzti2_(fraction);
    return (atan_frac128_t) {
        .fraction = fraction << lz,
        .exponent = input->exact_exponent - (int)lz,
    };
}

static __attribute__((cold, noinline)) long double
atan_accurate_exact_(u128 fraction, int exponent, u128 sign)
{
    u384_t wide = atan2_fraction_384_(U384(0, 0, fraction), exponent);
    unsigned lz = (unsigned)clzti2_(wide.limb[2]);
    return atan2_round_384_(u384_shl_(wide, lz), exponent - (int)lz, sign);
}

long double atanl(long double x)
{
    static const u128 one = (u128)F128_BIAS << F128_EXP_SHIFT;
    u128 bits = f128_bits_(x);
    u128 ax = bits & ~F128_SIGN_MASK;
    if (ax - 1 >= F128_EXP_MASK - 1)
        return atan2_edge_(bits, one);

    u128 sign = bits & F128_SIGN_MASK;
    atan_input_t input = atan_reduce_(ax);
    atan_frac128_t fast = atan_input_fast_(&input);
    long double result;
    if (atan2_round_fast_(fast.fraction, fast.exponent, sign, &result))
        return result;
    if (input.exact)
        return atan_accurate_exact_(input.exact_fraction,
            input.exact_exponent, sign);
    return atan2_accurate_(&input.reduction, sign);
}

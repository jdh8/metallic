/* Correctly rounded binary128 asin/acos.
 * Ported from metallic-rs 6c2178df4e987cd67e6541bd32843fd3031cb20a. */
#include "kernel/atan.h"
#include "kernel/roots.h"
#include <stdbool.h>
#include <stdint.h>

#define ASIN_BAND_ (-3)
#define ASIN_NARROW_BAND_ (-4)

static const u128 ASIN_ONE_ = (u128)F128_BIAS << F128_EXP_SHIFT;
static const u128 ASIN_PI_ = U128(0x4000921fb54442d1, 0x8469898cc51701b8);
static const u128 ASIN_PI_2_ = U128(0x3fff921fb54442d1, 0x8469898cc51701b8);

static const u128 ASIN_COEF_[19] = {
    U128(0x2aaaaaaaaaaaaaaa, 0xaaaaaaaaaaaaaaab),
    U128(0x1333333333333333, 0x3333333333333333),
    U128(0x0b6db6db6db6db6d, 0xb6db6db6db6db6db),
    U128(0x07c71c71c71c71c7, 0x1c71c71c71c71c72),
    U128(0x05ba2e8ba2e8ba2e, 0x8ba2e8ba2e8ba2e9),
    U128(0x04713b13b13b13b1, 0x3b13b13b13b13b14),
    U128(0x0393333333333333, 0x3333333333333333),
    U128(0x02f50f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f),
    U128(0x027fbca1af286bca, 0x1af286bca1af286c),
    U128(0x0225de79e79e79e7, 0x9e79e79e79e79e7a),
    U128(0x01df3bd37a6f4de9, 0xbd37a6f4de9bd37a),
    U128(0x01a6863d70a3d70a, 0x3d70a3d70a3d70a4),
    U128(0x01782dda12f684bd, 0xa12f684bda12f685),
    U128(0x0151ba308d3dcb08, 0xd3dcb08d3dcb08d4),
    U128(0x0131683bdef7bdef, 0x7bdef7bdef7bdef8),
    U128(0x0115ee9d45d1745d, 0x1745d1745d1745d1),
    U128(0x00fe57c7db6db6db, 0x6db6db6db6db6db7),
    U128(0x00e9e954706eb3e4, 0x5306eb3e45306eb4),
    U128(0x00d8137abd89d89d, 0x89d89d89d89d89d9),
};

typedef struct {
    u384_t vn;
    u128 q;
    u256_t frame;
    u256_t s;
    int es;
    int es_base;
} asin_sqrt_t;

double sqrt(double);

static inline bool asin_u256_zero_(u256_t x)
{
    return x.limb[0] == 0 && x.limb[1] == 0;
}

static inline bool asin_u384_zero_(u384_t x)
{
    return x.limb[0] == 0 && x.limb[1] == 0 && x.limb[2] == 0;
}

static u256_t asin_abs_sub_256_(u256_t a, u256_t b, bool *negative)
{
    *negative = b.limb[1] > a.limb[1]
        || (b.limb[1] == a.limb[1] && b.limb[0] > a.limb[0]);
    return *negative ? u256_sub_(b, a) : u256_sub_(a, b);
}

static u256_t asin_mul_small_256_(u256_t a, u128 k)
{
    u128 high;
    u128 low;
    wmul_(a.limb[0], k, &high, &low);
    return U256(low, a.limb[1] * k + high);
}

static u384_t asin_mul_small_384_(u384_t a, u128 k)
{
    u128 h0;
    u128 l0;
    u128 h1;
    u128 l1;
    wmul_(a.limb[0], k, &h0, &l0);
    wmul_(a.limb[1], k, &h1, &l1);
    u128 middle = l1 + h0;
    u128 carry = middle < l1;
    return U384(l0, middle, a.limb[2] * k + h1 + carry);
}

/* ceil((hi:lo) / 2^shift), with shift >= 1. */
static u384_t asin_ceil_shr_(u128 hi, u128 lo, unsigned shift)
{
    if (shift >= 256)
        return U384(1, 0, 0);

    u256_t floor = u256_shr_sat_(U256(lo, hi), shift);
    bool dropped;
    if (shift >= 128) {
        unsigned rem = shift - 128;
        dropped = lo != 0;
        if (rem)
            dropped |= (hi & (((u128)1 << rem) - 1)) != 0;
    } else {
        dropped = (lo & (((u128)1 << shift) - 1)) != 0;
    }
    u256_t rounded = u256_add_(floor, U256(dropped, 0));
    return U384(rounded.limb[0], rounded.limb[1], 0);
}

static asin_sqrt_t asin_wide_sqrt_(u128 m, int e)
{
    u128 hi;
    u128 lo;
    wmul_(m, m, &hi, &lo);
    int shift = 2 * e + 160;
    u384_t xx = shift >= 0
        ? u384_shl_(U384(lo, hi, 0), (unsigned)shift)
        : asin_ceil_shr_(hi, lo, (unsigned)-shift);
    u384_t v = u384_neg_(xx);
    unsigned lz = u384_clz_(v);
    unsigned parity = lz & ~1u;
    u384_t vn = u384_shl_(v, parity);
    u128 w = vn.limb[2];

    u128 seed = (u128)(0x1p158 / sqrt((double)(uint64_t)(w >> 64)));
    u128 q = rsqrt_step_(w, seed);
    u128 s0 = mhi_(w, q);
    u128 s0h = (s0 >> 126) == 0 ? s0 << 2 : ~(u128)0;

    u128 sqh;
    u128 sql;
    wmul_(s0h, s0h, &sqh, &sql);
    bool downward;
    u256_t h = asin_abs_sub_256_(U256(vn.limb[1], vn.limb[2]),
        U256(sql, sqh), &downward);
    u128 hs = (h.limb[1] << 94) | (h.limb[0] >> 34);
    u128 p1;
    u128 p0;
    wmul_(hs, q, &p1, &p0);
    u256_t correction = U256((p1 << 35) | (p0 >> 93), p1 >> 93);
    u256_t frame;
    if (downward) {
        frame = u256_sub_(U256(0, s0h), correction);
    } else {
        frame = u256_add_(U256(0, s0h), correction);
        if (frame.limb[1] < s0h)
            frame = U256(~(u128)0, ~(u128)0);
    }

    unsigned lzs = u256_clz_(frame);
    int es_base = -(int)(parity / 2);
    return (asin_sqrt_t) {
        .vn = vn,
        .q = q,
        .frame = frame,
        .s = u256_shl_(frame, lzs),
        .es = es_base - (int)lzs,
        .es_base = es_base,
    };
}

static u384_t asin_square_top_(u256_t frame)
{
    u128 h11;
    u128 l11;
    u128 h01;
    u128 l01;
    wmul_(frame.limb[1], frame.limb[1], &h11, &l11);
    wmul_(frame.limb[0], frame.limb[1], &h01, &l01);
    u384_t cross = U384(l01 << 1,
        (h01 << 1) | (l01 >> 127), h01 >> 127);
    return u384_add_(u384_add_(U384(0, l11, h11), cross),
        U384(mhi_(frame.limb[0], frame.limb[0]), 0, 0));
}

static u384_t asin_sqrt_384_(const asin_sqrt_t *sq, int *exponent)
{
    u384_t frame = U384(0, sq->frame.limb[0], sq->frame.limb[1]);
    u384_t square = asin_square_top_(sq->frame);
    bool downward = u384_cmp_(sq->vn, square) < 0;
    u384_t h = downward ? u384_sub_(square, sq->vn)
        : u384_sub_(sq->vn, square);
    u128 hs = (h.limb[1] << 32) | (h.limb[0] >> 96);
    u128 p1;
    u128 p0;
    wmul_(hs, sq->q, &p1, &p0);
    u384_t correction = U384((p1 << 97) | (p0 >> 31), p1 >> 31, 0);
    u384_t s3;
    if (downward) {
        s3 = u384_sub_(frame, correction);
    } else {
        s3 = u384_add_(frame, correction);
        if (s3.limb[2] < frame.limb[2])
            s3 = U384(~(u128)0, ~(u128)0, ~(u128)0);
    }
    unsigned lz = u384_clz_(s3);
    *exponent = sq->es_base - (int)lz;
    return u384_shl_(s3, lz);
}

/* round(64 * 2^-dn * n/d) half-up on the 15-bit tops; a sector off by one
 * only moves the residual within the polynomial's convergence window. */
static unsigned asin_sector_(u128 ntop, u128 dtop, unsigned dn)
{
    if (dn >= 8)
        return 0;
    uint64_t n = (uint64_t)(ntop >> 113) << 7 >> dn;
    uint64_t d = (uint64_t)(dtop >> 113);
    return (unsigned)((n + d) / (2 * d));
}

static atan_reduction_t asin_reduce_(u128 fx, int ex, const asin_sqrt_t *sq,
    bool acos, bool xneg)
{
    u256_t x = U256(0, fx);
    bool x_big = ex > sq->es || (ex == sq->es && fx > sq->s.limb[1]);
    u256_t d = x_big ? x : sq->s;
    u256_t n = x_big ? sq->s : x;
    int ed = x_big ? ex : sq->es;
    int en = x_big ? sq->es : ex;
    unsigned dn = (unsigned)(ed - en);
    unsigned sector = asin_sector_(n.limb[1], d.limb[1], dn);
    bool swap = x_big != acos;
    unsigned quadrant = swap ? 1 : 2 * (unsigned)xneg;
    bool negate = swap != xneg;

    u128 numerator;
    u128 denominator;
    bool negative = false;
    int scale;
    if (sector == 0) {
        numerator = n.limb[1];
        denominator = d.limb[1];
        scale = -(int)dn;
    } else {
        u256_t n14 = u256_shr_sat_(n, 14);
        u256_t d14 = u256_shr_sat_(d, 14);
        u256_t far = asin_mul_small_256_(d14, (u128)sector << dn);
        u256_t kn = asin_abs_sub_256_(u256_shl_(n14, 6), far, &negative);
        u256_t kd = u256_add_(u256_shl_(d14, 6 + dn),
            asin_mul_small_256_(n14, sector));
        unsigned lzd = u256_clz_(kd);
        if (asin_u256_zero_(kn)) {
            numerator = 0;
            denominator = u256_shl_(kd, lzd).limb[1];
            scale = 0;
        } else {
            unsigned lzn = u256_clz_(kn);
            numerator = u256_shl_(kn, lzn).limb[1];
            denominator = u256_shl_(kd, lzd).limb[1];
            scale = (int)lzd - (int)lzn;
        }
    }
    return (atan_reduction_t) {
        .numerator = numerator,
        .denominator = denominator,
        .scale = scale,
        .negative = negative,
        .sector = sector,
        .quadrant = quadrant,
        .negate = negate,
    };
}

static void asin_taylor_14_(u128 v, u128 *even, u128 *odd)
{
    uint64_t vh = v >> 64;
#define NARROW_(i) ((uint64_t)(ASIN_COEF_[i] >> 64))
    uint64_t te = NARROW_(8) + mul_hi_64_(vh,
        NARROW_(10) + mul_hi_64_(vh, NARROW_(12)));
    uint64_t to = NARROW_(7) + mul_hi_64_(vh,
        NARROW_(9) + mul_hi_64_(vh,
            NARROW_(11) + mul_hi_64_(vh, NARROW_(13))));
    *even = ASIN_COEF_[0] + mhi_approx_(v, ASIN_COEF_[2]
        + mhi_approx_(v, ASIN_COEF_[4] + mhi_approx_(v, ASIN_COEF_[6]
            + mhi_approx_(v, (u128)te << 64))));
    *odd = ASIN_COEF_[1] + mhi_approx_(v, ASIN_COEF_[3]
        + mhi_approx_(v, ASIN_COEF_[5]
            + mhi_approx_(v, (u128)to << 64)));
#undef NARROW_
}

static void asin_taylor_19_(u128 v, u128 *even, u128 *odd)
{
    uint64_t vh = v >> 64;
#define NARROW_(i) ((uint64_t)(ASIN_COEF_[i] >> 64))
    uint64_t te = NARROW_(10) + mul_hi_64_(vh,
        NARROW_(12) + mul_hi_64_(vh, NARROW_(14)
            + mul_hi_64_(vh, NARROW_(16)
                + mul_hi_64_(vh, NARROW_(18)))));
    uint64_t to = NARROW_(11) + mul_hi_64_(vh,
        NARROW_(13) + mul_hi_64_(vh,
            NARROW_(15) + mul_hi_64_(vh, NARROW_(17))));
    *even = ASIN_COEF_[0] + mhi_approx_(v, ASIN_COEF_[2]
        + mhi_approx_(v, ASIN_COEF_[4] + mhi_approx_(v, ASIN_COEF_[6]
            + mhi_approx_(v, ASIN_COEF_[8]
                + mhi_approx_(v, (u128)te << 64)))));
    *odd = ASIN_COEF_[1] + mhi_approx_(v, ASIN_COEF_[3]
        + mhi_approx_(v, ASIN_COEF_[5] + mhi_approx_(v, ASIN_COEF_[7]
            + mhi_approx_(v, ASIN_COEF_[9]
                + mhi_approx_(v, (u128)to << 64)))));
#undef NARROW_
}

static atan_frac128_t asin_series_(u128 t, int exponent)
{
    unsigned shift = (unsigned)(-2 * exponent);
    if (shift >= 128)
        return (atan_frac128_t) { t, exponent };
    u128 u = __metallic_atan_shr_round(atan_sqr_hi_(t), shift);
    u128 cube = mhi_approx_(t, u);
    u128 v = atan_sqr_hi_(u);
    u128 even;
    u128 odd;
    if (exponent <= ASIN_NARROW_BAND_)
        asin_taylor_14_(v, &even, &odd);
    else
        asin_taylor_19_(v, &even, &odd);
    u128 correction = mhi_approx_(cube, even + mhi_approx_(u, odd));
    u128 fraction = t + correction;
    bool carry = fraction < t;
    if (carry)
        return (atan_frac128_t) {
            .fraction = (fraction >> 1) | ((u128)1 << 127),
            .exponent = exponent + 1,
        };
    return (atan_frac128_t) { fraction, exponent };
}

static u384_t asin_recip_wide_(u384_t d)
{
    u384_t r = U384(0, 0, (atan_recip_128_(d.limb[2]) - 4) << 1);
    for (unsigned i = 0; i < 2; ++i) {
        u384_t residual = u384_sub_(U384(0, 0, (u128)1 << 127),
            u384_mul_hi_(d, r));
        u384_t correction = u384_shl_(u384_mul_hi_(r, residual), 1);
        r = u384_sub_(u384_add_(r, correction), U384(32, 0, 0));
    }
    return r;
}

static __attribute__((cold, noinline)) long double
asin_accurate_(u128 fx, int ex, const asin_sqrt_t *sq, bool acos,
    bool xneg, u128 sign)
{
    int es;
    u384_t s = asin_sqrt_384_(sq, &es);
    u384_t x = U384(0, 0, fx);
    bool x_big = ex > es || (ex == es && u384_cmp_(x, s) > 0);
    u384_t d = x_big ? x : s;
    u384_t n = x_big ? s : x;
    int ed = x_big ? ex : es;
    int en = x_big ? es : ex;
    unsigned dn = (unsigned)(ed - en);
    unsigned sector = asin_sector_(n.limb[2], d.limb[2], dn);
    bool swap = x_big != acos;
    unsigned quadrant = swap ? 1 : 2 * (unsigned)xneg;
    bool negate = swap != xneg;

    u384_t kn;
    u384_t kd;
    bool negative = false;
    int scale;
    if (sector == 0) {
        kn = n;
        kd = d;
        scale = -(int)dn;
    } else {
        u384_t n14 = u384_shr_sat_(n, 14);
        u384_t d14 = u384_shr_sat_(d, 14);
        u384_t far = asin_mul_small_384_(d14, (u128)sector << dn);
        u384_t six = u384_shl_(n14, 6);
        negative = u384_cmp_(far, six) > 0;
        kn = negative ? u384_sub_(far, six) : u384_sub_(six, far);
        kd = u384_add_(u384_shl_(d14, 6 + dn),
            asin_mul_small_384_(n14, sector));
        unsigned lzd = u384_clz_(kd);
        if (asin_u384_zero_(kn)) {
            kd = u384_shl_(kd, lzd);
            scale = 0;
        } else {
            unsigned lzn = u384_clz_(kn);
            kn = u384_shl_(kn, lzn);
            kd = u384_shl_(kd, lzd);
            scale = (int)lzd - (int)lzn;
        }
    }

    if (asin_u384_zero_(kn))
        return __metallic_atan_assemble_384(U384(0, 0, 0), negative, sector,
            quadrant, negate, sign);
    u384_t t = u384_mul_hi_(kn, asin_recip_wide_(kd));
    unsigned lz = (unsigned)clzti2_(t.limb[2]);
    t = u384_shl_(t, lz);
    int et = scale + 1 - (int)lz;
    u384_t fraction = __metallic_atan_fraction_384(t, et);
    if (sector == 0 && quadrant == 0) {
        unsigned lzf = (unsigned)clzti2_(fraction.limb[2]);
        return __metallic_atan_round_384(u384_shl_(fraction, lzf),
            et - (int)lzf, sign);
    }
    unsigned theta_shift = (unsigned)(3 - et);
    return __metallic_atan_assemble_384(u384_shr_sat_(fraction, theta_shift),
        negative, sector, quadrant, negate, sign);
}

static long double asin_arc_(u128 m, int e, u128 sign, bool acos, bool xneg)
{
    u128 fx = m << 15;
    int ex = e + 1;
    atan_frac128_t fast;
    asin_sqrt_t sq;
    bool have_sqrt = false;

    if (ex <= ASIN_BAND_) {
        fast = asin_series_(fx, ex);
        if (acos)
            fast = __metallic_atan_combine(__metallic_atan_place(fast.fraction,
                fast.exponent),
                0, false, 1, !xneg);
    } else {
        sq = asin_wide_sqrt_(m, e);
        have_sqrt = true;
        atan_reduction_t reduction = asin_reduce_(fx, ex, &sq, acos, xneg);
        fast = __metallic_atan_fast(&reduction);
    }

    long double result;
    if (__metallic_atan_round_fast(fast.fraction, fast.exponent, sign, &result))
        return result;
    if (!have_sqrt)
        sq = asin_wide_sqrt_(m, e);
    return asin_accurate_(fx, ex, &sq, acos, xneg, sign);
}

static __attribute__((cold, noinline)) long double
asin_edge_(u128 bits, u128 ax, u128 endpoint)
{
    if (ax == ASIN_ONE_)
        return f128_from_bits_(endpoint);
    if (ax > F128_EXP_MASK)
        return f128_from_bits_(bits | F128_QUIET_BIT);
    return f128_from_bits_(F128_EXP_MASK | F128_QUIET_BIT);
}

long double asinl(long double x)
{
    u128 bits = f128_bits_(x);
    u128 ax = bits & ~F128_SIGN_MASK;
    u128 sign = bits & F128_SIGN_MASK;
    if (ax == 0)
        return x;
    if (ax >= ASIN_ONE_)
        return asin_edge_(bits, ax, sign | ASIN_PI_2_);
    u128 m;
    int e;
    f128_split_(ax, &m, &e);
    return asin_arc_(m, e, sign, false, false);
}

long double acosl(long double x)
{
    u128 bits = f128_bits_(x);
    u128 ax = bits & ~F128_SIGN_MASK;
    bool xneg = (bits >> 127) != 0;
    if (ax == 0)
        return f128_from_bits_(ASIN_PI_2_);
    if (ax >= ASIN_ONE_)
        return asin_edge_(bits, ax, xneg ? ASIN_PI_ : 0);
    u128 m;
    int e;
    f128_split_(ax, &m, &e);
    return asin_arc_(m, e, 0, true, xneg);
}

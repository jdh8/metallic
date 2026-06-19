/* Correctly-rounded double sin/cos/tan — fast (double-double) path.
 *
 * Ported from metallic-rs (src/f64_/trig.rs).  The Rust original leans on
 * hardware FMA for its double-double products; here every product is a
 * Dekker two_prod via split_() instead, since WASM has no scalar FMA and the
 * library's fma() is a software routine (see CLAUDE.md, the math/ exception).
 *
 * Argument reduction returns a quadrant q and a reduced angle r in [-pi/4,
 * pi/4] as a double-double.  The lean kernels peel the exact leading Taylor
 * terms into double-double and evaluate only the tiny tail in plain f64,
 * reaching ~2^-60 — enough for the relative Ziv gate (TRIG_ZIV_EPS = 2^-59).
 * Inputs that fail the gate fall through to the 128-bit Dint path (trigdint.h).
 */
#ifndef METALLIC_KERNEL_TRIG_H
#define METALLIC_KERNEL_TRIG_H

#include "../../reinterpret.h"
#include "../split.h"
#include <math.h>
#include <stdint.h>

typedef struct {
    double hi, lo;
} dd_t;

/* Fast2Sum: requires |a| >= |b| (or exponent(a) >= exponent(b)). */
static inline dd_t trig_fast_sum_(double a, double b)
{
    double hi = a + b;
    return (dd_t){ hi, a - hi + b };
}

/* TwoSum: normalized pair from an unordered sum. */
static inline dd_t trig_from_sum_(double x, double y)
{
    double hi = x + y;
    double d = hi - x;
    return (dd_t){ hi, (y - d) + (d - hi + x) };
}

static inline dd_t trig_neg_(dd_t v)
{
    return (dd_t){ -v.hi, -v.lo };
}

/* dd * dd, breaking normality (high*high exact via Dekker, cross terms plain). */
static inline dd_t trig_mul_(dd_t a, dd_t b)
{
    double p = a.hi * b.hi;
    double ah = split_(a.hi), al = a.hi - ah;
    double bh = split_(b.hi), bl = b.hi - bh;
    double pl = ((ah * bh - p) + ah * bl + al * bh) + al * bl;
    double lo = (pl + a.hi * b.lo) + a.lo * b.hi;
    return (dd_t){ p, lo };
}

/* dd * f64. */
static inline dd_t trig_mul_f64_(dd_t a, double b)
{
    double p = a.hi * b;
    double ah = split_(a.hi), al = a.hi - ah;
    double bh = split_(b), bl = b - bh;
    double pl = ((ah * bh - p) + ah * bl + al * bh) + al * bl;
    return (dd_t){ p, pl + a.lo * b };
}

/* self + other, no renormalization, Fast2Sum on the high words.  Requires
 * exponent(self.hi) >= exponent(other.hi); the kernels' coefficient tables
 * guarantee this at the octant edge (proven in trig.rs's fold_ordering). */
static inline dd_t trig_add_ordered_(dd_t a, dd_t b)
{
    dd_t s = trig_fast_sum_(a.hi, b.hi);
    return (dd_t){ s.hi, s.lo + (a.lo + b.lo) };
}

static inline double trig_poly_(double x, const double *c, int n)
{
    double acc = c[n - 1];
    for (int i = n - 2; i >= 0; --i)
        acc = acc * x + c[i];
    return acc;
}

/* --- constants (trig.rs) --------------------------------------------------- */

/* Below 2^-27, sin(x)=tan(x)=x and cos(x)=1 to within half an ulp. */
#define TRIG_SMALL 7.450580596923828e-9

/* Relative half-width of the fast-path Ziv gate, 2^-59. */
#define TRIG_ZIV_EPS 1.734723475976807e-18

static const dd_t TRIG_ONE_DD = { 1.0, 0.0 };
static const dd_t TRIG_NEG_1_6 = { -0.16666666666666666, -9.25185853854297e-18 };
static const dd_t TRIG_1_120 = { 0.008333333333333333, 1.1564823173178714e-19 };
static const dd_t TRIG_1_24 = { 0.041666666666666664, 2.3129646346357427e-18 };

/* sin(r)/r = 1 - u/6 + u^2/120 + u^3*SIN_TAIL(u), u = r^2 (~2^-74). */
static const double TRIG_SIN_TAIL[6] = {
    -0.0001984126984126984,
    2.7557319223985634e-06,
    -2.505210838495563e-08,
    1.6059043500527755e-10,
    -7.647058546227501e-13,
    2.7962863666189794e-15,
};

/* cos(r) = 1 - u/2 + u^2/24 + u^3*COS_TAIL(u) (~2^-70). */
static const double TRIG_COS_TAIL[6] = {
    -0.001388888888888889,
    2.4801587301586814e-05,
    -2.755731922306274e-07,
    2.087675634918288e-09,
    -1.1470545821738428e-11,
    4.750660993437302e-14,
};

/* tan(r)/r = sum b_k v^k, v = r^2: 6 leading dd terms + v^6 tail. */
static const dd_t TRIG_TAN_LEADS[6] = {
    { 1.0, 0.0 },
    { 0.3333333333333333, 1.850371707708594e-17 },
    { 0.13333333333333333, 1.8503717077085942e-18 },
    { 0.05396825396825397, -2.5552752154071065e-18 },
    { 0.021869488536155203, -1.7377829530067485e-19 },
    { 0.008863235529902197, -7.63300580171831e-19 },
};

static const double TRIG_TAN_TAIL[15] = {
    0.003592128036572481,
    0.001455834387051282,
    0.0005900274409499761,
    0.0002391291140349121,
    9.691538474772203e-05,
    3.9278246728957075e-05,
    1.591965246822141e-05,
    6.44674860195779e-06,
    2.6377035355686352e-06,
    9.839484146788608e-07,
    6.078285342856009e-07,
    -1.204289917739238e-07,
    3.9928830670121667e-07,
    -2.0027017684620337e-07,
    9.276165196030918e-08,
};

/* Medium-range (|x| < 2^20) Cody-Waite reduction constants. */
#define TRIG_2_PI 0.6366197723675814
#define TRIG_PIO2_1 1.5707963267341256
#define TRIG_PIO2_2 6.077100506303966e-11
#define TRIG_PIO2_3 2.0222662487111665e-21
#define TRIG_PIO2_4 8.478427660348229e-32
static const dd_t TRIG_PIO2_DD = { 1.5707963267948966, 6.123233995736766e-17 };

/* Fractional bits of 2/pi, most-significant 64-bit group first.  Shared with
 * the Dint reduction in trigdint.h. */
static const uint64_t TRIG_FRAC_2_PI[24] = {
    0xA2F9836E4E441529, 0xFC2757D1F534DDC0, 0xDB6295993C439041, 0xFE5163ABDEBBC561,
    0xB7246E3A424DD2E0, 0x06492EEA09D1921C, 0xFE1DEB1CB129A73E, 0xE88235F52EBB4484,
    0xE99C7026B45F7E41, 0x3991D639835339F4, 0x9C845F8BBDF9283B, 0x1FF897FFDE05980F,
    0xEF2F118B5A0A6D1F, 0x6D367ECF27CB09B7, 0x4F463F669E5FEA2D, 0x7527BAC7EBE5F17B,
    0x3D0739F78A5292EA, 0x6BFB5FB11F8D5D08, 0x56033046FC7B6BAB, 0xF0CFBC209AF4361D,
    0xA9E391615EE61B08, 0x6599855F14A06840, 0x8DFFD8804D732731, 0x06061556CA73A8C9,
};

/* --- argument reduction ---------------------------------------------------- */

/* Bits [start, start+63] of the 5-limb little-endian value p (limb 0 = LSW);
 * limbs outside [0,5) read as zero.  start >= 0 here. */
static inline uint64_t trig_ph_bits_(const uint64_t p[5], int64_t start)
{
    int64_t w = start >> 6;
    int o = (int)(start & 63);
    uint64_t lo = (w >= 0 && w < 5) ? p[w] : 0;
    if (!o)
        return lo;
    uint64_t hi = (w + 1 >= 0 && w + 1 < 5) ? p[w + 1] : 0;
    return (lo >> o) | (hi << (64 - o));
}

/* Payne-Hanek reduction for x >= 2^20, returning a double-double r. */
static inline int64_t trig_payne_hanek_(double x, dd_t *out)
{
    uint64_t bits = reinterpret(uint64_t, x);
    int64_t e = (int64_t)((bits >> 52) & 0x7ff);
    unsigned __int128 sig = (unsigned __int128)((bits & 0x000FFFFFFFFFFFFF) | 0x0010000000000000);

    int64_t m = e - 1075;
    int64_t w0 = ((m - 2) > 0 ? (m - 2) : 0) / 64;
#define TRIG_W(i) ((uint64_t)(w0 + (i)) < 24 ? (unsigned __int128)TRIG_FRAC_2_PI[w0 + (i)] : 0)
    unsigned __int128 wa = TRIG_W(0), wb = TRIG_W(1), wc = TRIG_W(2), wd = TRIG_W(3);
#undef TRIG_W
    unsigned __int128 pa = sig * wa, pb = sig * wb, pc = sig * wc, pd = sig * wd;
    const unsigned __int128 MASK = 0xFFFFFFFFFFFFFFFFu;

    uint64_t p0 = (uint64_t)pd;
    unsigned __int128 c = (pd >> 64) + (pc & MASK);
    uint64_t p1 = (uint64_t)c;
    c = (c >> 64) + (pc >> 64) + (pb & MASK);
    uint64_t p2 = (uint64_t)c;
    c = (c >> 64) + (pb >> 64) + (pa & MASK);
    uint64_t p3 = (uint64_t)c;
    uint64_t p4 = (uint64_t)((c >> 64) + (pa >> 64));
    uint64_t p[5] = { p0, p1, p2, p3, p4 };

    int64_t b = 256 + 64 * w0 - m;
    unsigned __int128 frac = ((unsigned __int128)trig_ph_bits_(p, b - 64) << 64)
                           | trig_ph_bits_(p, b - 128);
    int64_t q = (int64_t)trig_ph_bits_(p, b);
    __int128 sfrac = (__int128)frac;
    int64_t quadrant = q - (int64_t)(sfrac >> 127);

    int64_t h = (int64_t)(sfrac >> 64);
    uint64_t l = (uint64_t)sfrac;
    double hf = (double)h;
    int64_t hr = h - (int64_t)hf;
    double lf = (double)hr * 0x1p64 + (double)l;
    dd_t fr = { hf * 0x1p-64, lf * 0x1p-128 };
    *out = trig_mul_(fr, TRIG_PIO2_DD);
    return quadrant;
}

/* x >= 0 finite -> (quadrant, r in [-pi/4, pi/4]). */
static inline int64_t trig_rem_pio2_(double x, dd_t *r)
{
    if (x < 1048576.0) {
        double q = rint(x * TRIG_2_PI);
        dd_t a = trig_from_sum_(x, q * -TRIG_PIO2_1);
        dd_t b = trig_from_sum_(a.hi, q * -TRIG_PIO2_2);
        dd_t c = trig_from_sum_(b.hi, q * -TRIG_PIO2_3);
        double lo = (-q) * TRIG_PIO2_4 + (a.lo + b.lo + c.lo);
        *r = trig_fast_sum_(c.hi, lo);
        return (int64_t)q;
    }
    return trig_payne_hanek_(x, r);
}

/* --- fast kernels ---------------------------------------------------------- */

static inline void trig_fast_squares_(dd_t r, dd_t *uu, dd_t *u2, double *u3)
{
    *uu = trig_mul_(r, r);
    *u2 = trig_mul_(*uu, *uu);
    double u = uu->hi;
    *u3 = u * u * u;
}

static inline dd_t trig_sin_part_fast_(dd_t r, dd_t uu, dd_t u2, double u3)
{
    dd_t lead = trig_add_ordered_(trig_add_ordered_(TRIG_ONE_DD, trig_mul_(uu, TRIG_NEG_1_6)),
                                  trig_mul_(u2, TRIG_1_120));
    double tail = u3 * trig_poly_(uu.hi, TRIG_SIN_TAIL, 6);
    dd_t sin_over_r = trig_add_ordered_(lead, (dd_t){ tail, 0.0 });
    return trig_mul_(r, sin_over_r);
}

static inline dd_t trig_cos_part_fast_(dd_t uu, dd_t u2, double u3)
{
    dd_t lead = trig_add_ordered_(trig_add_ordered_(TRIG_ONE_DD, trig_mul_f64_(uu, -0.5)),
                                  trig_mul_(u2, TRIG_1_24));
    double tail = u3 * trig_poly_(uu.hi, TRIG_COS_TAIL, 6);
    return trig_add_ordered_(lead, (dd_t){ tail, 0.0 });
}

static inline dd_t trig_sin_kernel_fast_(dd_t r)
{
    dd_t uu, u2;
    double u3;
    trig_fast_squares_(r, &uu, &u2, &u3);
    return trig_sin_part_fast_(r, uu, u2, u3);
}

static inline dd_t trig_cos_kernel_fast_(dd_t r)
{
    dd_t uu, u2;
    double u3;
    trig_fast_squares_(r, &uu, &u2, &u3);
    return trig_cos_part_fast_(uu, u2, u3);
}

static inline dd_t trig_tan_kernel_fast_(dd_t r)
{
    dd_t w = trig_mul_(r, r);
    double v = w.hi;
    double v2 = v * v;
    double v6 = v2 * v2 * v2;
    dd_t w2 = trig_mul_(w, w);
    dd_t p0 = trig_add_ordered_(TRIG_TAN_LEADS[0], trig_mul_(w, TRIG_TAN_LEADS[1]));
    dd_t p1 = trig_add_ordered_(TRIG_TAN_LEADS[2], trig_mul_(w, TRIG_TAN_LEADS[3]));
    dd_t p2 = trig_add_ordered_(TRIG_TAN_LEADS[4], trig_mul_(w, TRIG_TAN_LEADS[5]));
    double tail = v6 * trig_poly_(v, TRIG_TAN_TAIL, 15);
    dd_t inner = trig_add_ordered_(p1, trig_mul_(w2, p2));
    dd_t t = trig_add_ordered_(trig_add_ordered_(p0, trig_mul_(w2, inner)),
                               (dd_t){ tail, 0.0 });
    return trig_mul_(r, t);
}

/* Ziv gate: round v's high+low when both ends of its +-eps interval agree. */
static inline int trig_ziv_(dd_t v, double *out)
{
    double eps = fabs(v.hi) * TRIG_ZIV_EPS;
    double lo = v.hi + (v.lo - eps);
    double hi = v.hi + (v.lo + eps);
    if (lo == hi) {
        *out = lo;
        return 1;
    }
    return 0;
}

#endif

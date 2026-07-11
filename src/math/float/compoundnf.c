#include "powf.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

/* C23 compoundnf: (1 + x)^n, correctly rounded.
 *
 * Ported from metallic-rs f32_/pow.rs `compoundf` (~line 306): powf's
 * fast-f64 / double-double chain lifted onto the base 1 + x, plus an exact
 * integer skeleton for the short-dyadic results the double-double fallback
 * cannot decide.  The C23 exponent is a long long where metallic-rs takes an
 * (always f64-exact) f32, so |n| > 2^53 needs care: with |x| >= 2^-40 the
 * result is decided by sign alone, and the remaining tiny-x band carries n
 * exactly as a two-part nh + nl split through the fallback. */

/* log₂e as a double-double, the base-2 lift of log2p1_dd_ (f32_/pow.rs
 * LOG2E_DD). */
static const sum_t compound_log2e_ = { 0x1.71547652b82fep+0, 0x1.777d0ffda0d24p-56 };

/* log₂(1 + x) as a double-double, ~2⁻⁸⁰ relative (port of log2p1_dd,
 * f32_/pow.rs:133).  For |x| < 2⁻²⁹ the series log₂e·(x + x²(−½ + x/3 − x²/4));
 * otherwise 1 + x is formed exactly — an f32 mantissa plus the unit bit spans
 * at most 53 bits — and fed to log2_dd_; the one case that drops a bit is
 * x ≥ 2⁵³, where the residual c = 1 returns as the single low-word term. */
static sum_t log2p1_dd_(double x)
{
    if (fabs(x) < 0x1p-29) {
        static const double tail_c_[3] = { -0.5, 1.0 / 3.0, -0.25 };
        double tail = (x * x) * poly_f64_(x, tail_c_, 3);
        return dd_mul_dd_(compound_log2e_, dd_2sum_(x, tail));
    }

    double s, c;
    if (x <= 1.0) {
        s = 1.0 + x;
        c = x - (s - 1.0);
    } else {
        s = x + 1.0;
        c = 1.0 - (s - x);
    }

    sum_t d = log2_dd_(s);
    d.lo = (c / s) * 0x1.71547652b82fep+0 + d.lo;
    return d;
}

/* Exact integer square root, or 0 when m is not a perfect square.  m < 2^53
 * here, so the f64 sqrt seed is exact enough and r² fits a u64. */
static int exact_sqrt_(uint64_t m, uint64_t *out)
{
    uint64_t r = (uint64_t)llrint(sqrt((double)m));
    *out = r;
    return r * r == m;
}

/* The exact-or-midpoint results the double-double fallback cannot decide
 * (port of exact_compound, f32_/pow.rs:213).  A result with <= 25 significant
 * bits can sit exactly on an f32 value or midpoint (e.g. compoundnf of
 * x = 2⁴⁸+2²⁵, n... via the real-y corpus: (1+x)^½ ties); such results only
 * arise from an exactly-representable 1 + x = m·2^e and y = ±k·2^g with a tiny
 * integer skeleton, resolved here in exact integer arithmetic and rounded
 * once.  Inexact (double)n never fires: y > 34-ish magnitudes fail the k <= 15
 * gate and a power-of-two base needs an in-range e·y. */
static int exact_compound_(double x, double y, float *out)
{
    double s = 1.0 + x;
    double c = x <= 1.0 ? x - (s - 1.0) : 1.0 - (s - x);
    if (c != 0.0)
        return 0; /* 1 + x inexact ⇒ the result is never a short dyadic */

    /* s = m·2^e with m odd; s is a positive normal (x > −1, x ≠ 0). */
    uint64_t sb = reinterpret(uint64_t, s);
    uint64_t m = (sb & ((1ULL << 52) - 1)) | 1ULL << 52;
    int mt = __builtin_ctzll(m);
    m >>= mt;
    int64_t e = (int64_t)(sb >> 52) - 1075 + mt;

    /* y = ±k·2^g with k odd; y is finite and nonzero. */
    uint64_t yb = reinterpret(uint64_t, fabs(y));
    uint64_t k = (yb & ((1ULL << 52) - 1)) | 1ULL << 52;
    int kt = __builtin_ctzll(k);
    k >>= kt;
    int64_t g = (int64_t)(yb >> 52) - 1075 + kt;

    if (m == 1) {
        /* A power-of-two base: (2^e)^y = 2^(e·y), exact whenever e·y is an
         * integer (the f64 product of an 11-bit e and a 25-bit y is exact). */
        double t = (double)e * y;
        if (t != trunc(t) || !(t >= -160.0 && t <= 130.0))
            return 0; /* an irrational 2^t never lands on a boundary */
        *out = (float)shift_(1.0, (int64_t)t);
        return 1;
    }
    if (y < 0.0)
        return 0; /* 1/mᵏ with odd m ≥ 3 is never a dyadic rational */

    /* Fractional y: m (and e) must survive |g| exact square roots. */
    while (g < 0) {
        if (e % 2 != 0)
            return 0;
        if (!exact_sqrt_(m, &m))
            return 0;
        e /= 2;
        ++g;
    }

    /* Integer exponent k·2^g: m^k must fit 25 bits to matter (m ≥ 3 ⇒ k ≤ 15).
     * The p ≤ 2²⁵ cut also keeps p·m inside a u64: an m > 2²⁵ is rejected
     * after the first round, so later rounds multiply 25-bit by 25-bit. */
    if (g > 4 || k > 15)
        return 0;
    uint64_t kk = k << g;
    if (kk > 15)
        return 0;
    uint64_t p = 1;
    for (uint64_t i = 0; i < kk; ++i) {
        p *= m;
        if (p > 1ULL << 25)
            return 0;
    }
    int64_t ek = e * (int64_t)kk; /* |e| ≤ 1075, kk ≤ 15: no overflow */
    if (!(ek >= -160 && ek <= 130))
        return 0;
    *out = (float)shift_((double)p, ek);
    return 1;
}

/* (1 + x)^(nh + nl) for finite x > −1, x ≠ 0, exponent not in {0, 1} (port of
 * compoundf_core, f32_/pow.rs:161).  nh + nl carries the exponent exactly even
 * when (double)n rounds (|n| > 2^53, only reachable with |x| < 2^-40); the
 * rounded sum is plenty for the fast leg, whose Ziv distance absorbs the
 * 2^-53 slip, and the double-double fallback multiplies both parts. */
static float compoundnf_core_(double x, double nh, double nl)
{
    double y = nh + nl;
    double l = fabs(x) < 0x1p-29 ? 0x1.71547652b82fep+0 * ((-0.5 * x) * x + x)
                                 : log2_fast_(1.0 + x);
    double e = l * y;

    if (e > 130.0)
        return HUGE_VALF;
    if (e < -160.0)
        return 0.0f;

    double n = rint(e);
    double r = shift_(poly_f64_(e - n, exp2_fp_c_, 11), (int64_t)n);

    uint64_t rbits = reinterpret(uint64_t, r);
    int64_t dist = (int64_t)(rbits & UINT64_C(0x1FFFFFFF)) - INT64_C(0x10000000);
    if (dist < 0)
        dist = -dist;

    if (r >= (double)FLT_MIN && r < 0x1p127 && dist > 0x2000)
        return (float)r;

    float exact;
    if (exact_compound_(x, y, &exact))
        return exact;

    sum_t l2 = log2p1_dd_(x);
    sum_t edd = dd_mul_f64_(l2, nh);
    if (nl != 0.0)
        edd = dd_add_(edd, dd_mul_f64_(l2, nl));
    return exp2_dd_(edd);
}

float compoundnf(float x, long long n)
{
    if (x == 0.0f)
        return 1.0f; /* (1 ± 0)^n = 1 for every n */

    if (n == 0)
        return x < -1.0f ? (x - x) / (x - x) : 1.0f; /* domain error survives n = 0 */

    if (x != x)
        return x + x;

    if (x < -1.0f)
        return (x - x) / (x - x); /* 1 + x < 0, including −∞ */

    if (x == INFINITY)
        return n > 0 ? INFINITY : 0.0f;

    if (x == -1.0f)
        return n > 0 ? 0.0f : INFINITY;

    if (n == 1)
        /* 1 + x in f64 is either exact (|x| ≥ 2⁻⁵²) or rounds within
         * (1, 1 + 2⁻⁵²), whose f32 rounding is 1 either way. */
        return (float)(1.0 + (double)x);

    double y = (double)n;
    _Bool exact = y != 0x1p63 && (long long)y == n;

    if (!exact && fabsf(x) >= 0x1p-40f)
        /* |n| > 2^53 and |log₂(1+x)| > 2^-41: |e| > 2^12, far out of range. */
        return (x > 0.0f) == (n > 0) ? HUGE_VALF : 0.0f;

    double nh = y, nl = 0.0;
    if (!exact) {
        long long h = n & ~0x7FFLL; /* 53 significant bits; 0 ≤ n − h < 2^11 */
        nh = (double)h;
        nl = (double)(n - h);
    }
    return compoundnf_core_((double)x, nh, nl);
}

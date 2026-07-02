/* tgamma(x) = Γ(x), correctly rounded (<= 0.5 ulp).
 *
 * Ported from metallic-rs (src/f64_/gamma.rs), adapted for WASM in
 * kernel/gamma.h (hot double-double products use Dekker splits — fma() is a
 * software call here; only the near-DBL_MAX sites keep fma() residuals).
 *
 * Reduce z into [2.375, 3.375] (centre 2.875), evaluate Γ(2.875 + d) from a
 * per-cell minimax table, then walk the recurrence Γ(z) = Γ(z-i)·∏ up or down.
 * A double-double fast leg (table eval, Stirling for large z, reflection for
 * z<0) is Ziv-gated against a triple-double accurate path (degree-38 minimax
 * through a triple-double recurrence, ~2^-135) that clears the hardest ties.
 *
 * Special cases (C11/Annex F):
 *   tgamma(+inf)=+inf, tgamma(-inf)=NaN, tgamma(+-0)=+-inf,
 *   tgamma(negative integer)=NaN, overflow to +inf past ~171.62.
 */

#include "kernel/gamma.h"
#include <math.h>

typedef struct { double i; dd_t d; } reduce_t;
typedef struct { dd_t value; int64_t e2; } recur_t;
typedef struct { td_t value; int64_t e2; } recur_td_t;

/* Reduce z for the recurrence: zr = z - i in [2.375, 3.375]; Γ(z)=Γ(zr)·∏. */
static reduce_t tgamma_reduce_(double z)
{
    if (z >= 2.0 && z < TGAMMA_TABLE_HI)
        return (reduce_t){ 0.0, (dd_t){ z, 0.0 } };
    double i = rint(z - TGAMMA_CENTER);
    return (reduce_t){ i, exptab_twosum_(z, -i) };
}

/* Reduce z into [2.375,3.375] for the accurate path: d = z - (2.875 + i). */
static reduce_t tgamma_reduce_center_(double z)
{
    double i = rint(z - TGAMMA_CENTER);
    return (reduce_t){ i, exptab_twosum_(z, -(TGAMMA_CENTER + i)) };
}

/* Walk the recurrence: Γ(z) = value · 2^e2 from value = Γ(z-i). */
static recur_t tgamma_recurrence_(double z, double i, dd_t value)
{
    int64_t steps = (int64_t)fabs(i);

    /* The upward product ~ Γ(z)/Γ(zr) approaches DBL_MAX, past the ~2^996
     * Dekker-split bound, so this one combine keeps the fma() residual. */
    if (i > 0.0)
        return (recur_t){ gdd_mul_big_(value, g_recurrence_product_z_(z, -1, -1, steps)), 0 };
    if (i < 0.0 && steps <= TGAMMA_DOWNWARD_DIRECT)
        return (recur_t){ gdd_mul_(value, gdd_recip_(g_recurrence_product_z_(z, 0, 1, steps))), 0 };
    if (i < 0.0) {
        dd_t product = { z, 0.0 };
        int64_t e2 = 0;
        double k = 0.0;
        for (int64_t s = 1; s < steps; ++s) {
            k += 1.0;
            product = gdd_mul_(product, exptab_twosum_(z, k));
            if (fabs(product.hi) > PRODUCT_RESCALE) {
                product = (dd_t){ shift_(product.hi, -512), shift_(product.lo, -512) };
                e2 += 512;
            }
        }
        return (recur_t){ gdd_mul_(value, gdd_recip_(product)), -e2 };
    }
    return (recur_t){ value, 0 };
}

static recur_td_t tgamma_recurrence_td_(double z, double i, td_t value)
{
    int64_t steps = (int64_t)fabs(i);

    if (i > 0.0)
        return (recur_td_t){ g_td_mul_(value, g_recurrence_product_z_td_(z, -1, -1, steps)), 0 };
    if (i < 0.0 && steps <= TGAMMA_DOWNWARD_DIRECT)
        return (recur_td_t){ g_td_mul_(value, g_td_recip_(g_recurrence_product_z_td_(z, 0, 1, steps))), 0 };
    if (i < 0.0) {
        td_t product = { z, 0.0, 0.0 };
        int64_t e2 = 0;
        double k = 0.0;
        for (int64_t s = 1; s < steps; ++s) {
            k += 1.0;
            product = g_dd_mul_td_(exptab_twosum_(z, k), product);
            if (fabs(product.hi) > PRODUCT_RESCALE) {
                product = (td_t){ shift_(product.hi, -512), shift_(product.mid, -512), shift_(product.lo, -512) };
                e2 += 512;
            }
        }
        return (recur_td_t){ g_td_mul_(value, g_td_recip_(product)), -e2 };
    }
    return (recur_td_t){ value, 0 };
}

/* lnΓ(z) as a dd for z >= 36 (Stirling), feeding the exp fast leg. */
static dd_t tgamma_lngamma_stirling_(double z)
{
    dd_t inv = gdd_from_quotient_(1.0, z);
    double u = inv.hi * inv.hi;
    dd_t tail = gdd_add_ordered_(gdd_mul_(inv, TWELFTH),
        (dd_t){ inv.hi * (u * gpoly_(u, TGAMMA_STIRLING_TAIL_REST, 13)), 0.0 });

    dd_t base = gdd_mul_f64_(g_ln_dd_(z), z - 0.5);
    base = gdd_add_ordered_(base, (dd_t){ -z, 0.0 });
    base = gdd_add_ordered_(base, HALF_LN_2PI);
    return gdd_add_ordered_(base, tail);
}

static dd_t tgamma_stirling_fast_(double z, int64_t *q)
{
    return g_exp_dd_of_dd_fast_(tgamma_lngamma_stirling_(z), q);
}

static double tgamma_stirling_ziv_rel_(double z)
{
    return z * TGAMMA_STIRLING_ZIV_LIN + TGAMMA_STIRLING_ZIV_BASE;
}

/* Γ(1-z) from the wide table, cell-local arg formed exactly. */
static dd_t tgamma_reflect_gamma_(double z, double wf)
{
    double kf = rint(8.0 * wf);
    int64_t k = (int64_t)kf;
    const gamma_cell_t *cell = &TGAMMA_TABLE[k - TGAMMA_TABLE_KLO];
    double h = (1.0 - 0.125 * kf) - z;   /* (1 - k/8) - z, exact */
    return g_eval_cell_(cell, (dd_t){ h, 0.0 });
}

/* Magnitude |Γ(z)| = pi / (|sin(pi z)|·Γ(1-z)) via raw reciprocal-multiply,
 * always positive (the caller folds in sign(Γ(z)) = sign(sin πz)). */
static dd_t tgamma_reflect_value_(double z, double wf)
{
    dd_t denom = gdd_mul_(g_abs_sinpi_dd_lean_(z), tgamma_reflect_gamma_(z, wf));
    double rcp = 1.0 / denom.hi;
    double rh = rcp * PI_DD.hi;
    /* rh·denom.hi − π.hi exactly: Dekker product, then p.hi − π.hi is exact by
     * Sterbenz (rh·denom.hi ≈ π.hi), sparing the software fma. */
    dd_t p = exptab_prod_(rh, denom.hi);
    double resid = (p.hi - PI_DD.hi) + p.lo;
    double rl = rcp * ((PI_DD.lo - denom.lo * rh) - resid);
    return (dd_t){ rh, rl };
}

/* Reflection fast leg; returns 1 and *out on a certified Ziv hit, else 0. */
static int tgamma_reflect_fast_(double z, double *out)
{
    double wf = 1.0 - z;
    if (wf < 2.0 || wf >= TGAMMA_TABLE_HI)
        return 0;
    /* value is the positive magnitude, so the gate needs no fabs. */
    dd_t value = tgamma_reflect_value_(z, wf);
    double err = value.hi * TGAMMA_REFLECT_ZIV;
    double lo = value.hi + (value.lo - err);
    double hi = value.hi + (value.lo + err);
    if (lo != hi)
        return 0;
    /* sign(Γ(z)) = sign(sin πz): negative on the humps where ⌊z⌋ is odd.
     * Folded into the certified scalar as a sign-bit XOR: negation is exact
     * and commutes with round-to-nearest, so the bits match negating the
     * double-double before the gate — without the 50/50 parity branch that
     * mispredicts on random input (metallic-rs ffb264b).  z ∈ (−35, −1]
     * keeps the cast exact. */
    uint64_t sign = (uint64_t)((int64_t)floor(z) & 1) << 63;
    uint64_t bits = reinterpret(uint64_t, lo) ^ sign;
    *out = reinterpret(double, bits);
    return 1;
}

/* Accurate triple-double leg (degree-38 minimax + td recurrence). */
static double tgamma_accurate_(double z)
{
    reduce_t r = tgamma_reduce_center_(z);
    td_t seed = g_poly_td_(g_dd_to_td_(r.d), TGAMMA_TD, 39);
    recur_td_t rec = tgamma_recurrence_td_(z, r.i, seed);
    return g_round_td_signed64_(rec.value, rec.e2);
}

double tgamma(double z)
{
    if (isnan(z))
        return z;
    if (z == INFINITY)
        return INFINITY;

    /* Γ(+-0)=+-inf, and Γ(z)~1/z overflows for |z| <= 2^-1024. */
    if (fabs(z) <= TGAMMA_TINY)
        return copysign(INFINITY, z);

    if (z >= TGAMMA_OVERFLOW)
        return INFINITY;

    if (z < 0.0) {
        if (z == floor(z))           /* non-positive integer (and -inf) */
            return NAN;
        if (z <= TGAMMA_UNDERFLOW)   /* every hump rounds to a signed zero */
            return ((int64_t)floor(z) & 1) ? -0.0 : 0.0;
        double v;
        if (tgamma_reflect_fast_(z, &v))
            return v;
    }

    /* Large positive z: O(1) Stirling leg. */
    if (z >= TGAMMA_STIRLING_CUTOFF) {
        int64_t q;
        dd_t m = tgamma_stirling_fast_(z, &q);
        double err = m.hi * tgamma_stirling_ziv_rel_(z);
        double lo = m.hi + (m.lo - err);
        double hi = m.hi + (m.lo + err);
        if (lo == hi)
            return shift_(lo, q);
    }

    reduce_t red = tgamma_reduce_(z);
    recur_t rec = tgamma_recurrence_(z, red.i, g_tgamma_table_eval_(red.d));
    if (rec.e2 == 0) {
        dd_t value = rec.value;
        double err = fabs(value.hi) * TGAMMA_ZIV_EPS;
        double lo = value.hi + (value.lo - err);
        double hi = value.hi + (value.lo + err);
        if (lo == hi && fabs(lo) >= 0x1p-1021)
            return lo;
    }

    return tgamma_accurate_(z);
}

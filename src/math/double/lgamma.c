/* lgamma(x) = ln|Γ(x)|, correctly rounded (<= 0.5 ulp).
 *
 * Ported from metallic-rs (src/f64_/gamma.rs), adapted for WASM in
 * kernel/gamma.h (hot double-double products use Dekker splits — fma() is a
 * software call here; only the near-DBL_MAX sites keep fma() residuals).
 *
 * For 1/2 <= z < 8 a recurrence-free CORE-MATH piecewise minimax; for z >= 8
 * direct Stirling; for z < 1/2 the reflection ln π − ln|sin πz| − ln Γ(1−z).
 * Near the roots z = 1, 2 a result-anchored Taylor series carries the tiny
 * value with no cancellation.  A double-double fast leg is Ziv-gated against a
 * double-double middle tier and a triple-double accurate path (~2^-150), the
 * latter using a ported CORE-MATH triple-double ln and ln|sin πz|.
 *
 * Special cases (C11/Annex F):
 *   lgamma(1)=lgamma(2)=+0, lgamma(+-inf)=+inf, lgamma(+-0)=+inf,
 *   lgamma(non-positive integer)=+inf, overflow to +inf past ~2.5e305.
 */

#include "kernel/gamma.h"
#include <math.h>

#define LGAMMA_OVERFLOW 0x1.754d9278b51a8p+1014

typedef struct { dd_t value; double gate; } leg_t;

/* ── CORE-MATH piecewise lnΓ over [1/2, 8) ────────────────────────────────── */

/* Select the LGAMMA_PIECE region for ax in [0.5, 8.29541] (CORE-MATH hash). */
static int lgamma_piece_region_(double ax)
{
    uint32_t au = (uint32_t)((reinterpret(uint64_t, ax) << 1) >> 38);
    uint32_t ou = au - LGAMMA_PIECE_BORDERS[0];
    uint64_t t32 = (uint64_t)(uint32_t)(ou * 0x150du);
    uint64_t m = ((0x157ced865ull - t32) * (uint64_t)ou + 0x128000000000ull) >> 45;
    int j = (int)m;
    return j - (au < LGAMMA_PIECE_BORDERS[j] ? 1 : 0);
}

static leg_t lgamma_piecewise_(dd_t y)
{
    double ax = y.hi;
    int j = lgamma_piece_region_(ax);
    const dd_t *cell = LGAMMA_PIECE_CH[j];
    double z = ax - LGAMMA_PIECE_OFFS[j];
    double tail = z * gpoly_(z, LGAMMA_PIECE_CL[j], 8);
    dd_t value = c_polydddfst_(z, cell, 5, (dd_t){ tail, 0.0 });

    if (y.lo != 0.0) {
        double deriv = ((4.0 * cell[4].hi * z + 3.0 * cell[3].hi) * z + 2.0 * cell[2].hi) * z + cell[1].hi;
        value = exptab_add_(value, (dd_t){ deriv * y.lo, 0.0 });
    }
    if (j == 4)
        value = gdd_mul_(value, exptab_add_((dd_t){ 1.0, 0.0 }, gdd_neg_(y)));
    else if (j == 10)
        value = gdd_mul_(value, exptab_add_(y, (dd_t){ -2.0, 0.0 }));

    double gate = fabs(value.hi) * LGAMMA_PIECE_REL + LGAMMA_PIECE_ABS;
    return (leg_t){ value, gate };
}

/* Root-anchored series within 1/8 of z = 1 or 2; *out set & returns 1 on hit. */
static int lgamma_root_fast_(double z, leg_t *out)
{
    dd_t series;
    if (fabs(z - 1.0) < LGAMMA_ROOT_WINDOW)
        series = g_poly_dd_of_td_((dd_t){ z - 1.0, 0.0 }, LGAMMA_ROOT1_TD, 42);
    else if (fabs(z - 2.0) < LGAMMA_ROOT_WINDOW)
        series = g_poly_dd_of_td_((dd_t){ z - 2.0, 0.0 }, LGAMMA_ROOT2_TD, 34);
    else
        return 0;
    *out = (leg_t){ series, LGAMMA_ROOT_ZIV_REL * fabs(series.hi) };
    return 1;
}

static dd_t lgamma_stirling_fast_(double z, double tail)
{
    dd_t base = gdd_mul_f64_(g_ln_dd_(z), z - 0.5);
    base = gdd_add_ordered_(base, (dd_t){ -z, 0.0 });
    base = gdd_add_ordered_(base, HALF_LN_2PI);
    return gdd_add_ordered_(base, (dd_t){ tail, 0.0 });
}

static dd_t lgamma_stirling_dd_(dd_t y, double inv_t)
{
    double u = inv_t * inv_t;
    double tail = gpoly_(u, LGAMMA_TAIL_F64, 10) * inv_t;
    dd_t lw = g_ln_dd_(y.hi);
    dd_t base = gdd_mul_f64_(lw, y.hi - 0.5);
    base = gdd_add_ordered_(base, (dd_t){ -y.hi, 0.0 });
    base = gdd_add_ordered_(base, HALF_LN_2PI);
    base = gdd_add_ordered_(base, (dd_t){ tail, 0.0 });
    double digamma = lw.hi - inv_t * (u * (1.0 / 12.0) + 0.5);
    return (dd_t){ base.hi, digamma * y.lo + base.lo };
}

static leg_t lgamma_pos_fast_(dd_t y)
{
    if (y.hi < LGAMMA_FAST_CUTOFF)
        return lgamma_piecewise_(y);
    double inv_t = 1.0 / y.hi;
    return (leg_t){ lgamma_stirling_dd_(y, inv_t), g_lgamma_stirling_ziv_(y.hi, inv_t) };
}

static leg_t lgamma_fast_(double z)
{
    if (z < 0.5) {
        dd_t w = exptab_twosum_(1.0, -z);
        dd_t sinpi = g_abs_sinpi_dd_lean_(z);
        if (w.hi < LGAMMA_FAST_CUTOFF) {
            leg_t p = lgamma_piecewise_(w);
            dd_t ln_sin = g_ln_sum_dd_(sinpi);
            dd_t value = gdd_add_loose_(LN_PI, gdd_neg_(gdd_add_loose_(ln_sin, p.value)));
            double gate = fabs(ln_sin.hi) * LGAMMA_REFLECT_SIN_REL
                          + (2.0 * p.gate + LGAMMA_REFLECT_SIN_ABS);
            return (leg_t){ value, gate };
        }
        leg_t p = lgamma_pos_fast_(w);
        dd_t value = gdd_add_loose_(LN_PI, gdd_neg_(gdd_add_loose_(g_ln_sum_dd_(sinpi), p.value)));
        return (leg_t){ value, p.gate };
    }
    if (z < LGAMMA_FAST_CUTOFF) {
        leg_t r;
        if (lgamma_root_fast_(z, &r))
            return r;
        return lgamma_piecewise_((dd_t){ z, 0.0 });
    }
    double inv = 1.0 / z;
    double u = inv * inv;
    double tail = (z < LGAMMA_CUTOFF ? gpoly_(u, LGAMMA_TAIL_F64, 10)
                                     : gpoly_(u, LGAMMA_TAIL_F64_FAR, 6)) * inv;
    return (leg_t){ lgamma_stirling_fast_(z, tail), g_lgamma_stirling_ziv_(z, inv) };
}

/* ── double-double middle Ziv tier ────────────────────────────────────────── */

static dd_t lgamma_stirling_mid_(dd_t t)
{
    dd_t inv = gdd_recip_(t);
    dd_t u = gdd_mul_(inv, inv);
    dd_t tail = gdd_mul_(g_poly_dd_of_td_(u, LGAMMA_TAIL_TD, 10), inv);

    dd_t r = gdd_mul_(g_ln_sum_dd_(t), exptab_add_(t, (dd_t){ -0.5, 0.0 }));
    r = exptab_add_(r, gdd_neg_(t));
    r = exptab_add_(r, HALF_LN_2PI);
    return exptab_add_(r, tail);
}

static leg_t lgamma_pos_mid_(dd_t y)
{
    if (fabs(y.hi - 1.0) < LGAMMA_ROOT_WINDOW) {
        dd_t s = g_poly_dd_of_td_(exptab_add_(y, (dd_t){ -1.0, 0.0 }), LGAMMA_ROOT1_TD, 42);
        return (leg_t){ s, LGAMMA_ROOT_ZIV_REL * fabs(s.hi) };
    }
    if (fabs(y.hi - 2.0) < LGAMMA_ROOT_WINDOW) {
        dd_t s = g_poly_dd_of_td_(exptab_add_(y, (dd_t){ -2.0, 0.0 }), LGAMMA_ROOT2_TD, 34);
        return (leg_t){ s, LGAMMA_ROOT_ZIV_REL * fabs(s.hi) };
    }
    int64_t steps = (int64_t)fmax(ceil(LGAMMA_CUTOFF - y.hi), 0.0);
    dd_t t = exptab_add_(y, (dd_t){ (double)steps, 0.0 });
    dd_t stirling = lgamma_stirling_mid_(t);
    if (steps > 0) {
        dd_t ln_prod = g_ln_sum_dd_(g_recurrence_product_dd_(y, steps));
        double mag = fabs(stirling.hi) + fabs(ln_prod.hi);
        return (leg_t){ exptab_add_(stirling, gdd_neg_(ln_prod)), LGAMMA_MID_ZIV_REL * mag };
    }
    return (leg_t){ stirling, LGAMMA_MID_ZIV_REL * fabs(stirling.hi) };
}

static leg_t lgamma_mid_(double z)
{
    if (z < 0.5) {
        leg_t p = lgamma_pos_mid_(exptab_twosum_(1.0, -z));
        dd_t ln_sin = g_ln_sum_dd_(g_abs_sinpi_dd_(z));
        dd_t value = exptab_add_(LN_PI, gdd_neg_(exptab_add_(ln_sin, p.value)));
        double gate = LGAMMA_MID_ZIV_REL * fabs(ln_sin.hi) + p.gate;
        return (leg_t){ value, gate };
    }
    return lgamma_pos_mid_((dd_t){ z, 0.0 });
}

/* ── triple-double accurate path ──────────────────────────────────────────── */

static td_t lgamma_central_td_(dd_t d)
{
    return g_ln_sum_td3_(g_poly_td_(g_dd_to_td_(d), TGAMMA_TD, 39));
}

static td_t lgamma_recurrence_log_td_(dd_t y, int64_t i)
{
    if (i > 0) {
        dd_t yc = exptab_add_(y, (dd_t){ -(double)i, 0.0 });
        return g_ln_sum_td3_(g_recurrence_product_dd_td_(yc, i));
    }
    if (i < 0)
        return g_td_neg_(g_ln_sum_td3_(g_recurrence_product_dd_td_(y, -i)));
    return (td_t){ 0.0, 0.0, 0.0 };
}

static td_t lgamma_pos_td_(dd_t y)
{
    if (y.hi < LGAMMA_FAST_CUTOFF) {
        if (fabs(y.hi - 1.0) < LGAMMA_ROOT_WINDOW)
            return g_poly_td_(g_dd_to_td_(exptab_add_(y, (dd_t){ -1.0, 0.0 })), LGAMMA_ROOT1_TD, 42);
        if (fabs(y.hi - 2.0) < LGAMMA_ROOT_WINDOW)
            return g_poly_td_(g_dd_to_td_(exptab_add_(y, (dd_t){ -2.0, 0.0 })), LGAMMA_ROOT2_TD, 34);
        double fi = rint(y.hi - LGAMMA_CENTER);
        int64_t i = (int64_t)fi;
        dd_t d = exptab_add_(y, (dd_t){ -(LGAMMA_CENTER + fi), 0.0 });
        return g_td_add_exact_(lgamma_central_td_(d), lgamma_recurrence_log_td_(y, i));
    }

    int64_t steps = (int64_t)fmax(ceil(LGAMMA_CUTOFF - y.hi), 0.0);
    dd_t t = exptab_add_(y, (dd_t){ (double)steps, 0.0 });

    td_t inv_t = g_td_recip_(g_dd_to_td_(t));
    td_t u = g_td_mul_(inv_t, inv_t);
    td_t tail = g_td_mul_(g_poly_td_(u, LGAMMA_TAIL_TD, 10), inv_t);

    td_t ln_t_m1 = g_td_add_f64_(g_ln_sum_td_(t), -1.0);
    td_t t_m_half = g_dd_to_td_(exptab_add_(t, (dd_t){ -0.5, 0.0 }));
    td_t stirling = g_td_add_(g_td_add_(g_td_mul_(ln_t_m1, t_m_half), HALF_LN_2PI_M_HALF_TD), tail);

    if (steps > 0)
        return g_td_add_exact_(stirling, g_td_neg_(g_ln_sum_td3_(g_recurrence_product_dd_td_(y, steps))));
    return stirling;
}

static double lgamma_accurate_(double z)
{
    if (z < 0.5) {
        td_t ln_sin = g_ln_abs_sinpi_td_(z);
        td_t pos = lgamma_pos_td_(exptab_twosum_(1.0, -z));
        return g_td_round_(g_td_add_exact_(LN_PI_TD, g_td_neg_(g_td_add_exact_(ln_sin, pos))));
    }
    return g_td_round_(lgamma_pos_td_((dd_t){ z, 0.0 }));
}

double lgamma(double z)
{
    if (z == 0.0 || z >= LGAMMA_OVERFLOW)
        return INFINITY;
    if (isnan(z))
        return z;

    /* Non-positive integers are poles; Γ(1)=Γ(2)=1 give the exact zeros. */
    if (z < 0.5 && z == floor(z))
        return INFINITY;
    if (z == 1.0 || z == 2.0)
        return 0.0;

    /* Near a negative integer / tiny z the fast leg's dd sin underflows; the
     * gate cannot see it, so skip straight to the accurate path there. */
    int sin_ok = z >= 0.5;
    if (!sin_ok) {
        double r = z - rint(2.0 * z) * 0.5;
        sin_ok = 3.14159265358979323846 * fabs(r) >= LGAMMA_SIN_RELIABLE;
    }

    if (sin_ok && z > -LGAMMA_FAST_BOUND && z < LGAMMA_FAST_BOUND) {
        leg_t f = lgamma_fast_(z);
        double lo = f.value.hi + (f.value.lo - f.gate);
        double hi = f.value.hi + (f.value.lo + f.gate);
        if (lo == hi)
            return lo;

        leg_t m = lgamma_mid_(z);
        lo = m.value.hi + (m.value.lo - m.gate);
        hi = m.value.hi + (m.value.lo + m.gate);
        if (lo == hi)
            return lo;
    }

    return lgamma_accurate_(z);
}

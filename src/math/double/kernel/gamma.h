#ifndef METALLIC_KERNEL_GAMMA_DOUBLE_H
#define METALLIC_KERNEL_GAMMA_DOUBLE_H

/* Correctly-rounded f64 tgamma/lgamma machinery, ported from metallic-rs
 * (src/f64_/gamma.rs) and adapted for WASM: no scalar FMA, so every
 * double-double product uses Dekker splits (reusing exptab.h's primitives)
 * rather than fma().  See .claude/skills/program-math-functions.
 *
 * Layers:
 *   - double-double (dd_t)    : fast/middle Ziv legs (~2^-66 .. ~2^-105)
 *   - triple-double (td_t)    : accurate Ziv fallback (~2^-135 .. ~2^-150)
 * gated so any value a leg certifies is the correctly-rounded result the
 * accurate path would have returned. */

#include "../../reinterpret.h"
#include "exptab.h"   /* dd primitives (exptab_*), exptab_mantissa_, exptab_sum_ */
#include "logtab.h"   /* logtab_ln_dd_ : accurate dd natural log */
#include <math.h>
#include <stdint.h>

typedef exptab_sum_ dd_t;                       /* { double hi, lo; } */
typedef struct { double hi, mid, lo; } td_t;    /* non-overlapping triple */
typedef struct { dd_t c0, c1, c2, c3; double tail[8]; } gamma_cell_t;

#include "gammatab.h"

/* ── double-double ops ──────────────────────────────────────────────────────
 * twosum/fast2sum/add come from exptab.h (addition never overflows via a split).
 * Products use fma() for the residual instead of Dekker's split: the split's
 * (2^27+1)*x overflows for |x| >~ 2^996, which the gamma recurrence/Stirling
 * products reach (Γ approaches DBL_MAX).  fma() is software here but correct and
 * overflow-free; the hot table path only ever forms tiny products anyway. */

static inline dd_t gdd_neg_(dd_t a) { return (dd_t){ -a.hi, -a.lo }; }

/* exact product x*y as a dd (fma residual, no split overflow) */
static inline dd_t gprod_(double x, double y)
{
    double hi = x * y;
    return (dd_t){ hi, fma(x, y, -hi) };
}

/* dd * dd */
static inline dd_t gdd_mul_(dd_t a, dd_t b)
{
    dd_t p = gprod_(a.hi, b.hi);
    double lo = fma(a.hi, b.lo, p.lo);
    lo = fma(a.lo, b.hi, lo);
    return (dd_t){ p.hi, lo };
}

/* dd * scalar */
static inline dd_t gdd_mul_f64_(dd_t a, double b)
{
    dd_t p = gprod_(a.hi, b);
    return (dd_t){ p.hi, fma(a.lo, b, p.lo) };
}

/* a + b with a Fast2Sum on the high words (requires |a.hi| >= |b.hi|), no renorm */
static inline dd_t gdd_add_ordered_(dd_t a, dd_t b)
{
    dd_t s = exptab_fast2sum_(a.hi, b.hi);
    return (dd_t){ s.hi, s.lo + (a.lo + b.lo) };
}

/* a + b with a 2Sum on the high words (any magnitudes), no renorm */
static inline dd_t gdd_add_loose_(dd_t a, dd_t b)
{
    dd_t s = exptab_twosum_(a.hi, b.hi);
    return (dd_t){ s.hi, s.lo + (a.lo + b.lo) };
}

/* 1 / a via one Newton step from the f64 seed 1/a.hi */
static inline dd_t gdd_recip_(dd_t a)
{
    double y = 1.0 / a.hi;
    dd_t t = gdd_mul_f64_(a, -y);
    t = exptab_add_(t, (dd_t){ 2.0, 0.0 });
    return gdd_mul_f64_(t, y);
}

/* x / y as a dd (x, y plain f64) */
static inline dd_t gdd_from_quotient_(double x, double y)
{
    double hi = x / y;
    return (dd_t){ hi, fma(hi, -y, x) / y };
}

/* Horner over plain f64 coefficients, low-degree first */
static inline double gpoly_(double x, const double *c, int n)
{
    double s = c[n - 1];
    for (int i = n - 2; i >= 0; --i)
        s = s * x + c[i];
    return s;
}

/* ── triple-double ops ────────────────────────────────────────────────────── */

static inline td_t g_renorm3_(double a, double b, double c)
{
    dd_t t = exptab_twosum_(b, c);
    dd_t u = exptab_twosum_(a, t.hi);
    dd_t v = exptab_twosum_(u.lo, t.lo);
    return (td_t){ u.hi, v.hi, v.lo };
}

static inline td_t g_td_add_f64_(td_t t, double x)
{
    dd_t s0 = exptab_twosum_(t.hi, x);
    dd_t s1 = exptab_twosum_(t.mid, s0.lo);
    return g_renorm3_(s0.hi, s1.hi, t.lo + s1.lo);
}

static inline td_t g_td_add_(td_t a, td_t b)
{
    return g_td_add_f64_(g_td_add_f64_(g_td_add_f64_(a, b.hi), b.mid), b.lo);
}

static inline td_t g_td_neg_(td_t t) { return (td_t){ -t.hi, -t.mid, -t.lo }; }

static inline td_t g_dd_to_td_(dd_t d) { return (td_t){ d.hi, d.lo, 0.0 }; }

static inline td_t g_dd_mul_td_(dd_t d, td_t t)
{
    dd_t q0 = gprod_(d.hi, t.hi);
    dd_t q1 = gprod_(d.hi, t.mid);
    dd_t q2 = gprod_(d.lo, t.hi);
    td_t acc = { q0.hi, 0.0, 0.0 };
    double terms[7] = { q1.hi, q2.hi, q0.lo, q1.lo, q2.lo, d.hi * t.lo, d.lo * t.mid };
    for (int i = 0; i < 7; ++i)
        acc = g_td_add_f64_(acc, terms[i]);
    return acc;
}

static inline td_t g_td_mul_(td_t a, td_t b)
{
    dd_t p_hh = gprod_(a.hi, b.hi);
    dd_t p_hm = gprod_(a.hi, b.mid);
    dd_t p_mh = gprod_(a.mid, b.hi);
    td_t acc = { p_hh.hi, 0.0, 0.0 };
    double terms[8] = { p_hm.hi, p_mh.hi, p_hh.lo, p_hm.lo, p_mh.lo,
                        a.hi * b.lo, a.mid * b.mid, a.lo * b.hi };
    for (int i = 0; i < 8; ++i)
        acc = g_td_add_f64_(acc, terms[i]);
    return acc;
}

static inline td_t g_td_recip_(td_t t)
{
    dd_t seed = gdd_recip_((dd_t){ t.hi, t.mid });
    td_t s = g_dd_to_td_(seed);
    td_t two = { 2.0, 0.0, 0.0 };
    return g_td_mul_(s, g_td_add_(two, g_td_neg_(g_td_mul_(t, s))));
}

/* round normalized triple-double to nearest f64 (round-to-odd of mid+low) */
static inline double g_td_round_(td_t t)
{
    dd_t se = exptab_twosum_(t.mid, t.lo);
    double s = se.hi, e = se.lo;
    if (e != 0.0 && (reinterpret(uint64_t, s) & 1u) == 0) {
        uint64_t bits = reinterpret(uint64_t, s);
        int up = ((s >= 0.0) == (e > 0.0));
        s = reinterpret(double, up ? bits + 1 : bits - 1);
    }
    return t.hi + s;
}

/* round signed value*2^n to nearest f64, subnormal-safe */
static inline double g_round_td_signed64_(td_t value, int64_t n)
{
    double sign = value.hi;
    td_t mag = sign < 0.0 ? g_td_neg_(value) : value;

    int64_t e = (int64_t)(reinterpret(uint64_t, mag.hi) >> 52) - 1023;
    td_t m = { shift_(mag.hi, -e), shift_(mag.mid, -e), shift_(mag.lo, -e) };
    int64_t shift = e + n;

    double magnitude;
    if (shift >= -1022) {
        magnitude = shift_(g_td_round_(m), shift);
    } else {
        int64_t s = shift + 1074;
        double h = shift_(m.hi, s), mid = shift_(m.mid, s), lo = shift_(m.lo, s);
        double n0 = rint(h);
        td_t resid = { h - n0, mid, lo };
        double r = g_td_round_(g_td_add_f64_(resid, 0.0));
        magnitude = (n0 + rint(r)) * 0x1p-1074;
    }
    return copysign(magnitude, sign);
}

/* Estrin evaluation of a td polynomial (coeffs low-degree first) */
static inline td_t g_poly_td_(td_t u, const td_t *coeffs, int n)
{
    td_t buf[44];
    for (int i = 0; i < n; ++i)
        buf[i] = coeffs[i];
    int len = n;
    td_t power = u;
    while (len > 1) {
        int half = (len + 1) / 2;
        for (int i = 0; i < half; ++i)
            buf[i] = (2 * i + 1 < len)
                ? g_td_add_(buf[2 * i], g_td_mul_(power, buf[2 * i + 1]))
                : buf[2 * i];
        len = half;
        power = g_td_mul_(power, power);
    }
    return buf[0];
}

/* ── accurate dd natural log; lean ln re-used from logtab ──────────────────── */

static inline dd_t g_ln_dd_(double x) { return logtab_ln_dd_(x); }

/* ── exp of a dd exponent -> mantissa in [1,2) + binary exponent q ─────────── */

static inline dd_t g_exp_dd_of_dd_fast_(dd_t w, int64_t *qout)
{
    double scaled = rint(w.hi * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;
    int j = (int)(n & (EXPTAB_N - 1));
    int64_t q = n >> 7;

    double a = w.hi - scaled * exptab_ln2_over_n_hi_;   /* exact */
    dd_t blo = exptab_prod_(scaled, exptab_ln2_over_n_lo_);
    dd_t r = exptab_twosum_(a, -blo.hi);
    r = exptab_fast2sum_(r.hi, r.lo - blo.lo - scaled * exptab_ln2_over_n_lo2_ + w.lo);

    dd_t m = exptab_mantissa_(j, &q, r);
    *qout = q;
    return m;
}

/* ── |sin(pi*x)| as a dd, lean table-driven kernel (~2^-66) ────────────────── */

static const dd_t GAMMA_SINPI_S0 = { 0.02454369260617026, 9.567553118338697e-19 };
static const dd_t GAMMA_SINPI_S1 = { -2.4641574764489986e-6, 1.0807811177153324e-22 };
static const double GAMMA_SINPI_S2 = 7.421954185344935e-11;
static const double GAMMA_SINPI_S3 = -1.064507645268961e-15;
static const dd_t GAMMA_SINPI_C1 = { -0.0003011964233730883, -1.9120164516417576e-20 };
static const double GAMMA_SINPI_C2 = 1.5119880908790113e-8;
static const double GAMMA_SINPI_C3 = -3.036036034369748e-13;

static inline dd_t g_abs_sinpi_dd_lean_(double x)
{
    double a = fabs(x - rint(x));
    double s = a * 128.0;
    double kf = rint(s);
    double d = s - kf;
    int k = (int)kf;
    dd_t sg = GAMMA_SINPI_TABLE[k];        /* sin(pi*k/128) */
    dd_t cg = GAMMA_SINPI_TABLE[64 - k];   /* cos(pi*k/128) */

    double d2 = d * d;
    double sp[3] = { GAMMA_SINPI_S1.hi, GAMMA_SINPI_S2, GAMMA_SINPI_S3 };
    dd_t sin_over_d = gdd_add_ordered_(GAMMA_SINPI_S0, (dd_t){ d2 * gpoly_(d2, sp, 3), 0.0 });
    double cp[2] = { GAMMA_SINPI_C2, GAMMA_SINPI_C3 };
    dd_t cosm1 = gdd_add_ordered_(GAMMA_SINPI_C1, (dd_t){ d2 * gpoly_(d2, cp, 2), 0.0 });

    dd_t bracket = exptab_add_(gdd_mul_(cg, sin_over_d),
                               gdd_mul_(sg, gdd_mul_f64_(cosm1, d)));
    return exptab_add_(sg, gdd_mul_f64_(bracket, d));
}

/* ── per-cell minimax evaluator (shared by the gamma table & lgamma table) ─── */

static inline dd_t g_eval_cell_(const gamma_cell_t *cell, dd_t h)
{
    double z = h.hi;
    double tail = gpoly_(z, cell->tail, 8);
    dd_t acc = gdd_add_ordered_(cell->c3, (dd_t){ z * tail, 0.0 });
    acc = gdd_add_ordered_(cell->c2, gdd_mul_f64_(acc, z));
    acc = gdd_add_ordered_(cell->c1, gdd_mul_f64_(acc, z));
    dd_t value = gdd_add_ordered_(cell->c0, gdd_mul_f64_(acc, z));

    if (h.lo == 0.0)
        return value;
    double deriv = z * (z * (3.0 * cell->c3.hi) + 2.0 * cell->c2.hi) + cell->c1.hi;
    return (dd_t){ value.hi, deriv * h.lo + value.lo };
}

/* ── tgamma constants ─────────────────────────────────────────────────────── */

#define TGAMMA_CENTER 2.875
#define TGAMMA_TABLE_KLO 16
#define TGAMMA_TABLE_HI 36.0
#define TGAMMA_OVERFLOW 171.6243769563027
#define TGAMMA_TINY 5.562684646268003e-309
#define TGAMMA_UNDERFLOW (-179.5)
#define TGAMMA_DOWNWARD_DIRECT 100
#define TGAMMA_STIRLING_CUTOFF 36.0

static const double TGAMMA_ZIV_EPS = 2.168404344971009e-19;            /* 2^-62 */
static const double TGAMMA_STIRLING_ZIV_LIN = 2.710505431213761e-20;   /* 2^-65 */
static const double TGAMMA_STIRLING_ZIV_BASE = 5.421010862427522e-20;  /* 2^-64 */
static const double TGAMMA_REFLECT_ZIV = 0x1p-61;
static const double PRODUCT_RESCALE = 0x1p512;

static const dd_t TWELFTH = { 0.08333333333333333, 4.625929269271485e-18 };
static const dd_t HALF_LN_2PI = { 0.9189385332046728, -3.8782941580672414e-17 };
static const dd_t PI_DD = { 3.141592653589793, 1.2246467991473532e-16 };

/* Stirling tail B_k/(2k(2k-1)) for k=2..14 (after the leading 1/12). */
static const double TGAMMA_STIRLING_TAIL_REST[13] = {
    -0.002777777777777778, 0.0007936507936507937, -0.0005952380952380953,
    0.0008417508417508417, -0.0019175269175269176, 0.00641025641025641,
    -0.029550653594771242, 0.17964437236883057, -1.3924322169059011,
    13.402864044168393, -156.84828462600203, 2193.1033333333335,
    -36108.77125372499,
};

/* ── tgamma table evaluation & recurrence products ────────────────────────── */

static inline dd_t g_tgamma_table_eval_(dd_t zr)
{
    double kf = rint(zr.hi * 8.0);
    int64_t k = (int64_t)kf;
    const gamma_cell_t *cell = &TGAMMA_TABLE[k - TGAMMA_TABLE_KLO];
    return g_eval_cell_(cell, (dd_t){ zr.hi - kf * 0.125, zr.lo });
}

/* prod_{m=0}^{n-1} (z + first + step*m) as a dd, four parallel lanes */
static inline dd_t g_recurrence_product_z_(double z, int64_t first, int64_t step, int64_t n)
{
    dd_t p0 = { 1.0, 0.0 }, p1 = { 1.0, 0.0 }, p2 = { 1.0, 0.0 }, p3 = { 1.0, 0.0 };
    int64_t m = 0;
    for (; m + 4 <= n; m += 4) {
        p0 = gdd_mul_(p0, exptab_twosum_(z, (double)(first + step * m)));
        p1 = gdd_mul_(p1, exptab_twosum_(z, (double)(first + step * (m + 1))));
        p2 = gdd_mul_(p2, exptab_twosum_(z, (double)(first + step * (m + 2))));
        p3 = gdd_mul_(p3, exptab_twosum_(z, (double)(first + step * (m + 3))));
    }
    for (; m < n; ++m)
        p0 = gdd_mul_(p0, exptab_twosum_(z, (double)(first + step * m)));
    return gdd_mul_(gdd_mul_(p0, p1), gdd_mul_(p2, p3));
}

static inline td_t g_recurrence_product_z_td_(double z, int64_t first, int64_t step, int64_t n)
{
    td_t p0 = { 1.0, 0.0, 0.0 }, p1 = { 1.0, 0.0, 0.0 },
         p2 = { 1.0, 0.0, 0.0 }, p3 = { 1.0, 0.0, 0.0 };
    int64_t m = 0;
    for (; m + 4 <= n; m += 4) {
        p0 = g_dd_mul_td_(exptab_twosum_(z, (double)(first + step * m)), p0);
        p1 = g_dd_mul_td_(exptab_twosum_(z, (double)(first + step * (m + 1))), p1);
        p2 = g_dd_mul_td_(exptab_twosum_(z, (double)(first + step * (m + 2))), p2);
        p3 = g_dd_mul_td_(exptab_twosum_(z, (double)(first + step * (m + 3))), p3);
    }
    for (; m < n; ++m)
        p0 = g_dd_mul_td_(exptab_twosum_(z, (double)(first + step * m)), p0);
    return g_td_mul_(g_td_mul_(p0, p1), g_td_mul_(p2, p3));
}

#endif

#include "../reinterpret.h"
#include "kernel/exptab.h" /* exptab_sum_, exptab_prod_, exptab_fast2sum_; pulls in shift_ */
#include "normalize.h"
#include <math.h>
#include <stdint.h>

/* Correctly-rounded double cbrt, ported from metallic-rs (f64 `cbrt`, CORE-MATH
 * mantissa-reduction redesign + division-free Ziv fast leg).
 *
 * Reduce |x| = z·2ᵉ (z ∈ [1,2)), split e = 3·et + it, work on zz = z·2^it ∈
 * [1,8).  All intermediates are O(1) — no magnitude-scaling branch needed.
 * The Ziv fast leg avoids division; the accurate leg uses Dekker dd helpers. */

/* x/y as a double-double, FMA-free. */
static inline exptab_sum_ cbrt_from_quotient_(double x, double y)
{
    double hi = x / y;
    exptab_sum_ p = exptab_prod_(hi, y);
    double lo = ((x - p.hi) - p.lo) / y;
    return (exptab_sum_){ hi, lo };
}

/* Double-double s divided by scalar y, FMA-free. */
static inline exptab_sum_ cbrt_div_(exptab_sum_ s, double y)
{
    double hi = s.hi / y;
    exptab_sum_ p = exptab_prod_(hi, y);
    double lo = (((s.hi - p.hi) - p.lo) + s.lo) / y;
    return (exptab_sum_){ hi, lo };
}

/* Degree-3 minimax of z^(1/3) on [1,2), max error < 9.2e-5 (CORE-MATH cr_cbrt). */
static const double cbrt_c_[4] = {
     0.5528234184016472,
     0.5871142918266982,
    -0.16296967194987905,
     0.023104964110781470,
};

/* 2^(it/3) for it ∈ {0,1,2}: folds e mod 3 into the mantissa root. */
static const double cbrt_escale_[3] = {
    1.0,
    1.2599210498948732,  /* 2^(1/3) */
    1.5874010519681994,  /* 2^(2/3) */
};

/* 2^(−it) for it ∈ {0,1,2}: converts 1/z to 1/zz for the linear Newton. */
static const double cbrt_rsc_[3] = { 1.0, 0.5, 0.25 };

double cbrt(double x)
{
    int64_t bits = reinterpret(int64_t, x);
    int64_t magnitude = bits & 0x7FFFFFFFFFFFFFFF;

    if (magnitude == 0 || magnitude >= 0x7FF0000000000000)
        return x; /* +-0, +-inf, nan */

    magnitude = normalize_(magnitude);
    int64_t sign = bits & (int64_t)0x8000000000000000;

    /* Decompose |x| = z·2ᵉ, z ∈ [1,2).  Split e = 3·et + it (Euclidean
     * division, it ∈ {0,1,2}) so cbrt(|x|) = cbrt(z·2^it)·2^et. */
    int64_t e = (magnitude >> 52) - 1023;
    int it = (int)(((e % 3) + 3) % 3);     /* non-negative e mod 3 */
    int64_t et = (e - it) / 3;

    double z = reinterpret(double, (magnitude & INT64_C(0x000FFFFFFFFFFFFF))
                                   | INT64_C(0x3FF0000000000000));
    int64_t zz_bits = reinterpret(int64_t, z) + ((int64_t)it << 52);
    double zz = reinterpret(double, zz_bits);

    /* Forward seed: degree-3 minimax polynomial + one cubic Newton step.
     * 1/z is issued first so its latency overlaps the polynomial evaluation. */
    double r = 1.0 / z;
    double y = (cbrt_c_[3] * z + cbrt_c_[2]) * (z * z)
             + (cbrt_c_[1] * z + cbrt_c_[0]);
    double h = (y * y) * (y * r) - 1.0;
    y -= y * h * (1.0/3.0 - (2.0/9.0) * h);
    y *= cbrt_escale_[it];  /* y ≈ zz^(1/3) ∈ [1, 2) */

    /* One linear Newton step against the exact cube y³ (via Dekker products),
     * division-free using rr = 1/zz:  dy = (y³ − zz) · rr · y · (1/3). */
    exptab_sum_ y2e = exptab_prod_(y, y);             /* exact y² = y2e.hi + y2e.lo */
    exptab_sum_ y3e = exptab_prod_(y2e.hi, y);        /* exact y²_hi · y */
    double y3l    = y3e.lo + y * y2e.lo;              /* y³ low word */
    double rr     = r * cbrt_rsc_[it];                /* 1/zz */
    double hh     = ((y3e.hi - zz) + y3l) * rr;
    double dy     = hh * (y * (1.0/3.0));
    double y1     = y - dy;
    double low    = (y - y1) - dy;                    /* y1 + low ≈ zz^(1/3), ≈2⁻⁷⁷ */

    /* Ziv gate: seed+cubicNewton reaches ≈2⁻⁴⁰; one linearNewton → ≈2⁻⁷⁷.
     * 2⁻⁷³ bounds it with margin.  The 2^et scale is exact so gate the pair. */
    double err = shift_(y1, -73);   /* 2⁻⁷³ · y1 (y1 ∈ [1,2)) */
    double lo  = y1 + (low - err);
    double hi  = y1 + (low + err);
    if (lo == hi)
    {
        int64_t r_bits = sign | reinterpret(int64_t, shift_(lo, (int)et));
        return reinterpret(double, r_bits);
    }

    /* Accurate leg: double-double cube-root Newton (2·y1 + zz/y1²)/3 from ≈1-ulp
     * y1, then scale by 2^et. */
    exptab_sum_ quotient = cbrt_div_(cbrt_from_quotient_(zz, y1), y1);
    exptab_sum_ sum = exptab_fast2sum_(2.0 * y1, quotient.hi);
    sum = cbrt_div_((exptab_sum_){ sum.hi, quotient.lo + sum.lo }, 3.0);
    int64_t r_bits = sign | reinterpret(int64_t, shift_(sum.hi + sum.lo, (int)et));
    return reinterpret(double, r_bits);
}

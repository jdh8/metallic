#ifndef METALLIC_MATH_FLOAT_DD_H
#define METALLIC_MATH_FLOAT_DD_H

/* Double-double arithmetic kit shared by the correctly-rounded float kernels.
 *
 * A `sum_t` carries an unevaluated hi + lo pair with |lo| <= ulp(hi)/2, giving
 * ~106 bits of significand.  These are the building blocks the float functions
 * use on their rare Ziv slow paths: compute the result to double-double, then
 * cast to float with round-to-odd (round_dd_) so the single float rounding is
 * correct even at a midpoint.
 *
 * WASM has no hardware FMA -- `fma` is a software routine here -- so keep these
 * on the slow path only; the fast path should use plain double arithmetic. */

#include "../reinterpret.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

typedef struct { double hi, lo; } sum_t;

static inline sum_t dd_2sum_(double a, double b)
{
    double hi = a + b;
    double diff = hi - a;
    double lo = (a - (hi - diff)) + (b - diff);
    return (sum_t){ hi, lo };
}

static inline sum_t dd_product_(double a, double b)
{
    double hi = a * b;
    return (sum_t){ hi, fma(a, b, -hi) };
}

static inline sum_t dd_quotient_(double a, double b)
{
    double hi = a / b;
    return (sum_t){ hi, fma(hi, -b, a) / b };
}

static inline sum_t dd_add_(sum_t a, sum_t b)
{
    sum_t s = dd_2sum_(a.hi, b.hi);
    double lo = s.lo + (a.lo + b.lo);
    /* Fast2Sum: |s.hi| >= |lo| guaranteed after the 2Sum */
    double hi = s.hi + lo;
    return (sum_t){ hi, lo - (hi - s.hi) };
}

static inline sum_t dd_mul_dd_(sum_t a, sum_t b)
{
    sum_t p = dd_product_(a.hi, b.hi);
    double lo = fma(a.hi, b.lo, p.lo);
    lo = fma(a.lo, b.hi, lo);
    return (sum_t){ p.hi, lo };
}

static inline sum_t dd_mul_f64_(sum_t a, double b)
{
    sum_t p = dd_product_(a.hi, b);
    return (sum_t){ p.hi, fma(a.lo, b, p.lo) };
}

/* sqrt of a non-negative double-double, ~106-bit accurate.  One Newton-Heron
 * step refines the f64 sqrt of the high word: r + (S - r^2) / (2r). */
static inline sum_t dd_sqrt_(sum_t s)
{
    if (s.hi <= 0.0)
        return (sum_t){ 0.0, 0.0 };

    double r = sqrt(s.hi);
    sum_t r2 = dd_product_(r, r);
    sum_t res = dd_add_(s, (sum_t){ -r2.hi, -r2.lo });
    double corr = (res.hi + res.lo) / (2.0 * r);
    return dd_2sum_(r, corr);
}

/* a / b for double-double a, b, ~106-bit accurate. */
static inline sum_t dd_div_(sum_t a, sum_t b)
{
    double q = a.hi / b.hi;
    sum_t qb = dd_mul_f64_(b, q);
    sum_t res = dd_add_(a, (sum_t){ -qb.hi, -qb.lo });
    double corr = (res.hi + res.lo) / b.hi;
    return dd_2sum_(q, corr);
}

/* Horner evaluation of a double-double polynomial at a double-double argument.
 * coeffs[0] is the constant term; coeffs[n-1] is the leading term. */
static inline sum_t poly_dd_(sum_t u, const sum_t c[], int n)
{
    sum_t acc = c[n - 1];
    for (int k = n - 2; k >= 0; --k)
        acc = dd_add_(dd_mul_dd_(acc, u), c[k]);
    return acc;
}

/* Round a positive normal-range double-double to the nearest float.
 * Uses round-to-odd on the f64 high word to prevent double-rounding at
 * float midpoints (every midpoint has >= 28 trailing zero bits in f64). */
static inline float round_dd_(sum_t v)
{
    uint64_t bits = reinterpret(uint64_t, v.hi);
    double odd;
    if (v.lo == 0.0 || (bits & 1))
        odd = v.hi;
    else if (v.lo > 0.0)
        odd = reinterpret(double, bits + 1);
    else
        odd = reinterpret(double, bits - 1);
    return (float)odd;
}

/* Like round_dd_ but safe for subnormal outputs (v.hi < FLT_MIN). */
static inline float round_dd_general_(sum_t v)
{
    if (v.hi >= (double)FLT_MIN)
        return round_dd_(v);

    /* Quantize on the 2^-149 subnormal grid to avoid double-rounding. */
    double h = v.hi * 0x1p149;
    double l = v.lo * 0x1p149;
    double n = rint(h);
    n += rint((h - n) + l);
    return (float)(n * 0x1p-149);
}

#endif

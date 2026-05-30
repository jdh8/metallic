#include "kernel/atanf.h"
#include "../double/kernel/atantab.h"
#include <math.h>
#include <stddef.h>

/* pi and pi/2 as double-double constants, giving ~2^-106 accuracy.
 * The low words correct for the rounding error in the high words. */
static const double atan2f_pi_hi_  = 0x1.921fb54442d18p+1;
static const double atan2f_pi_lo_  = 0x1.1a62633145c07p-53;
static const double atan2f_pi2_hi_ = 0x1.921fb54442d18p+0;
static const double atan2f_pi2_lo_ = 0x1.1a62633145c07p-54;

/* pi and pi/2 as 128-bit dint constants (same mantissa, exponent differs by 1). */
static const dint_t atan2f_pi_dint_ = { 0, 1,
    ((unsigned __int128)0xc90fdaa22168c234ULL << 64) | 0xc4c6628b80dc1cd1ULL };
static const dint_t atan2f_pi2_dint_ = { 0, 0,
    ((unsigned __int128)0xc90fdaa22168c234ULL << 64) | 0xc4c6628b80dc1cd1ULL };

/* Round the double-double (hi + lo) to the nearest float.
 *
 * The pair satisfies |lo| <= ulp(hi)/2.  A plain (float)(hi+lo) can
 * double-round when hi+lo lands within half a float ulp of a midpoint.
 * Instead we cast hi to float, compute the exact residual (hi-f)+lo,
 * and bump f by one float ulp when the residual shows we crossed the midpoint. */
static inline float dd_to_float_(double hi, double lo)
{
    float f = (float)hi;
    double residual = (hi - (double)f) + lo;

    if (residual == 0.0)
        return f;

    if (residual > 0.0) {
        float up = nextafterf(f, HUGE_VALF);
        double half_ulp = ((double)up - (double)f) * 0.5;
        return residual > half_ulp ? up : f;
    } else {
        float dn = nextafterf(f, -HUGE_VALF);
        double half_ulp = ((double)f - (double)dn) * 0.5;
        return -residual > half_ulp ? dn : f;
    }
}

/* Error bound for the fast path.  The computation is either:
 *   (a) kernel_atanf_(q)          for the acute branch, or
 *   (b) pi_dd - kernel_atanf_(q)  for the pi/pi2 branches,
 * where q = fl(y/x) is the rounded double quotient.  Error sources:
 *   division rounding:  |q - exact_ratio| <= 0.5 ulp(q)
 *   kernel approximation: |kernel(q) - atan(q)| <= ~1 ulp(q)  (for |q|<=1)
 *   Lipschitz:  |atan'| <= 1 propagates the division error
 *   pi_dd error: ~3e-33 (negligible)
 * Total: <= 1.5 ulp(q) <= 1.5 * 2^-52 < 2^-51 for |q| <= 1.
 * We trigger the dint fallback when the fast-path gap < ATAN2F_ERR. */
#define ATAN2F_ERR 0x1p-51

/* Exact double-double quotient |num|/|den| for float-valued num, den.
 * Since |den| has at most 24 significant bits, both halves of the Dekker
 * split of q_hi have at most 27 bits, making q_hi_half * |den| exact in double.
 * Returns q_hi = fl(|num|/|den|) and q_lo such that q_hi+q_lo = |num|/|den|. */
static inline void atan2f_exact_quot_(double abs_num, double abs_den,
                                      double *q_hi, double *q_lo)
{
    double q = abs_num / abs_den;
    double c = (0x1p27 + 1.0) * q;
    double q_h = c - (c - q);
    double q_l = q - q_h;
    /* Exact remainder: abs_num - q*abs_den = abs_num - q_h*abs_den - q_l*abs_den. */
    double r = (abs_num - q_h * abs_den) - q_l * abs_den;
    *q_hi = q;
    *q_lo = r / abs_den;
}

/* Return |residual| - half_ulpf(f, residual_direction).
 * Positive means the fast path is unambiguous; < ATAN2F_ERR triggers fallback. */
static inline double atan2f_gap_(float f, double residual)
{
    float adj = nextafterf(f, residual > 0.0 ? HUGE_VALF : -HUGE_VALF);
    double half = fabs((double)adj - (double)f) * 0.5;
    return fabs(fabs(residual) - half);
}

/* Accurate fallback: use 128-bit dint arithmetic to determine the correct float.
 *
 * abs_q_hi, abs_q_lo: exact double-double of the absolute value of the quotient
 *                     (|x/y| for pi/2 branch, |y/|x|| for acute/pi branches).
 * q_neg:  1 iff the signed quotient is negative.
 * base:   dint for the pi/pi2 offset, or NULL for the pure-atan acute branch.
 * hi:     the fast-path double result (sign bit gives the output sign).
 * residual: (hi - (float)hi) + lo from the fast path (lo=0 for acute branch).
 */
static float atan2f_accurate_(double abs_q_hi, double abs_q_lo, int q_neg,
                               const dint_t *base, double hi, double residual)
{
    /* 128-bit atan of the positive quotient. */
    dint_t atan_q = atantab_atan_dd_dint_(abs_q_hi, abs_q_lo);

    dint_t r;
    if (base == NULL) {
        /* Acute branch: result = |atan(q)|. */
        r = atan_q;
    } else {
        /* base - atan(q) where atan(q) = sgn(q) * atan(|q|).
         * q_neg means q < 0, so atan(q) = -atan_q (negate the dint).
         * Then: base - atan(q) = base + atan_q  when q < 0,
         *                       = base - atan_q  when q >= 0. */
        dint_t atan_signed = atan_q;
        if (q_neg)
            atan_signed.sgn = !atan_signed.sgn;
        dint_t neg_atan = { !atan_signed.sgn, atan_signed.ex, atan_signed.m };
        r = dint_add_(base, &neg_atan);
    }

    /* r holds |result| to 128 bits.  Decide between the two float candidates. */
    float f = (float)hi;
    float f_abs = fabsf(f);

    /* The other float neighbour (by absolute value).
     * residual > 0 means the true value is above hi in the signed sense.
     * For positive hi that means larger magnitude; for negative hi, smaller. */
    float f_other_abs;
    if ((residual > 0.0) == (hi > 0.0))
        f_other_abs = nextafterf(f_abs, HUGE_VALF);   /* larger magnitude */
    else
        f_other_abs = nextafterf(f_abs, 0.0f);        /* smaller magnitude */

    double midpt_d = 0.5 * ((double)f_abs + (double)f_other_abs);
    dint_t midpt = dint_from_f64_(midpt_d);
    int cmp = dint_cmp_magnitude_(&r, &midpt);

    float result_abs;
    if (cmp > 0)
        result_abs = f_other_abs > f_abs ? f_other_abs : f_abs;
    else if (cmp < 0)
        result_abs = f_other_abs < f_abs ? f_other_abs : f_abs;
    else if (base == NULL) {
        /* Acute branch, cmp == 0: dint gives |atan(q)| = midpoint to 128 bits.
         * Since |atan(q)| < |q| strictly for q != 0, and the midpoint is ~= |q|,
         * the true |atan(q)| < midpoint, so round toward smaller magnitude. */
        result_abs = f_other_abs < f_abs ? f_other_abs : f_abs;
    } else {
        /* pi/pi2 branch, cmp == 0: true value is transcendental, never exactly
         * equal to a rational midpoint, so the dint has resolved it.  The dint
         * rounded to nearest (128-bit), so cmp==0 means within 2^{-255} of the
         * midpoint.  Round-to-even as a last resort (this should be unreachable
         * in practice for float inputs). */
        uint32_t fa = reinterpret(uint32_t, f_abs);
        result_abs = (fa & 1u) ? f_other_abs : f_abs;
    }

    return copysignf(result_abs, f);
}

static float atan2f_(double y, double x)
{
    double a = fabs(x);
    double b = fabs(y);

    if (a < b) {
        /* pi/2 branch: copysign(pi/2, y) - atan(x/y). */
        double c    = copysign(atan2f_pi2_hi_, y);
        double c_lo = copysign(atan2f_pi2_lo_, y);
        double k    = kernel_atanf_(x / y);
        double hi   = c - k;
        double lo   = ((c - hi) - k) + c_lo;

        float f = (float)hi;
        double residual = (hi - (double)f) + lo;

        /* Apply Ziv only for finite nonzero k (k==0: result is +-pi/2 exact;
         * k infinite or NaN: special case, let dd_to_float_ handle it). */
        if (isfinite(k) && k != 0.0 && atan2f_gap_(f, residual) < ATAN2F_ERR) {
            double qh, ql;
            atan2f_exact_quot_(a, b, &qh, &ql);
            return atan2f_accurate_(qh, ql, x < 0.0, &atan2f_pi2_dint_,
                                    hi, residual);
        }
        return dd_to_float_(hi, lo);
    }

    /* |x| >= |y|: compute atan(y/|x|) then apply quadrant correction. */
    double acute;
    if (a == b) {
        /* Equal magnitudes — includes atan2(0,0) and the +-pi/4 / +-3pi/4 cases. */
        acute = b ? copysign(0x1.921fb54442d18p-1, y) : y;
    } else {
        acute = kernel_atanf_(y / a);
    }

    if (!signbit(x))
        /* First/fourth quadrant or x=+inf: result is acute. */
        goto acute_branch;

    {
        /* Second/third quadrant: copysign(pi, y) - atan(y/|x|). */
        double c    = copysign(atan2f_pi_hi_, y);
        double c_lo = copysign(atan2f_pi_lo_, y);
        double hi   = c - acute;
        double lo   = ((c - hi) - acute) + c_lo;

        float f = (float)hi;
        double residual = (hi - (double)f) + lo;

        /* Apply Ziv only when acute is finite and nonzero. */
        if (isfinite(acute) && acute != 0.0 && atan2f_gap_(f, residual) < ATAN2F_ERR) {
            double qh, ql;
            atan2f_exact_quot_(b, a, &qh, &ql);
            return atan2f_accurate_(qh, ql, y < 0.0, &atan2f_pi_dint_,
                                    hi, residual);
        }
        return dd_to_float_(hi, lo);
    }

acute_branch:;
    {
        float f = (float)acute;
        double residual = acute - (double)f;

        /* Apply Ziv only for finite nonzero acute (avoids zero, inf, NaN edge cases). */
        if (isfinite(acute) && f != 0.0f && atan2f_gap_(f, residual) < ATAN2F_ERR) {
            double qh, ql;
            atan2f_exact_quot_(b, a, &qh, &ql);
            return atan2f_accurate_(qh, ql, y < 0.0, NULL, acute, residual);
        }
        return f;
    }
}

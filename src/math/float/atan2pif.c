/* C23 atan2pif: atan2(y, x)/pi, correctly rounded.
 *
 * Structure of metallic-rs `atan2pif` (src/f32_/atan.rs) on this library's
 * atan2f machinery: every special ray is an exact dyadic in half-turns
 * (+-1/4 and +-3/4 at infinite corners, +-1/2 on the axes, +-0/+-1 at zero
 * y), so the bases of the quadrant folds carry no rounding error at all --
 * simpler than the radians version.  The interior runs atan2f's fast
 * kernel, lifts the angle by a double-double 1/pi, and defers ambiguous
 * roundings to the 128-bit dint tier with the same lift. */

#include "atan2f.h"
#include "../double/split.h"

static const double atan2pif_inv_pi_hi_ = 0x1.45f306dc9c883p-2;
static const double atan2pif_inv_pi_lo_ = -0x1.6b01ec5417056p-56;

/* 1/pi to 128 bits (dint value = m/2^128 * 2^(ex+1)). */
static const dint_t atan2pif_inv_pi_dint_ = { 0, -2,
    ((unsigned __int128)0xa2f9836e4e441529ULL << 64) | 0xfc2757d1f534ddc1ULL };

/* 1/2 and 1 in half-turns -- exact dyadic bases for the quadrant folds. */
static const dint_t atan2pif_half_dint_ = { 0, -1, (unsigned __int128)1 << 127 };
static const dint_t atan2pif_one_dint_  = { 0, 0, (unsigned __int128)1 << 127 };

/* k * (1/pi) as an unevaluated hi + lo pair: Dekker product with the
 * double-double constant (no FMA on wasm; the splits of the constant fold
 * at compile time). */
static inline void atan2pif_scale_(double k, double *hi, double *lo)
{
    double ph = k * atan2pif_inv_pi_hi_;
    double kh = split_(k), kl = k - kh;
    double ch = split_(atan2pif_inv_pi_hi_), cl = atan2pif_inv_pi_hi_ - ch;
    double e = (kh * ch - ph + kh * cl + kl * ch) + kl * cl;

    *hi = ph;
    *lo = e + k * atan2pif_inv_pi_lo_;
}

/* atan2f_accurate_ with the half-turn lift: atan(|q|) is computed to 128
 * bits, multiplied by the 128-bit 1/pi, then folded against the exact
 * dyadic base.  Interior atan2pi values are irrational (Niven's theorem:
 * atan(q)/pi is rational only for q in {0, +-1, +-inf}, all handled exactly
 * upstream), so the midpoint comparison can never land a true tie. */
static float atan2pif_accurate_(double abs_q_hi, double abs_q_lo, int q_neg,
                                const dint_t *base, double hi, double residual)
{
    dint_t atan_q = atantab_atan_dd_dint_(abs_q_hi, abs_q_lo);
    dint_t atanpi_q = dint_mul_(&atan_q, &atan2pif_inv_pi_dint_);

    dint_t r;
    if (base == NULL) {
        r = atanpi_q;
    } else {
        dint_t atan_signed = atanpi_q;
        if (q_neg)
            atan_signed.sgn = !atan_signed.sgn;
        dint_t neg_atan = { !atan_signed.sgn, atan_signed.ex, atan_signed.m };
        r = dint_add_(base, &neg_atan);
    }

    float f = (float)hi;
    float f_abs = fabsf(f);

    float f_other_abs;
    if ((residual > 0.0) == (hi > 0.0))
        f_other_abs = nextafterf(f_abs, HUGE_VALF);
    else
        f_other_abs = nextafterf(f_abs, 0.0f);

    double midpt_d = 0.5 * ((double)f_abs + (double)f_other_abs);
    dint_t midpt = dint_from_f64_(midpt_d);
    int cmp = dint_cmp_magnitude_(&r, &midpt);

    float result_abs;
    if (cmp > 0)
        result_abs = f_other_abs > f_abs ? f_other_abs : f_abs;
    else if (cmp < 0)
        result_abs = f_other_abs < f_abs ? f_other_abs : f_abs;
    else {
        /* Unreachable by irrationality; round to even as a last resort. */
        uint32_t fa = reinterpret(uint32_t, f_abs);
        result_abs = (fa & 1u) ? f_other_abs : f_abs;
    }

    return copysignf(result_abs, f);
}

float atan2pif(float y, float x)
{
    if (x != x || y != y)
        return x + y;

    double xd = x, yd = y;
    double a = fabs(xd);
    double b = fabs(yd);

    if (a < b) {
        /* copysign(1/2, y) - atan(x/y)/pi */
        double c = copysign(0.5, yd);
        double k = kernel_atanf_(xd / yd);
        double kp_hi, kp_lo;
        atan2pif_scale_(k, &kp_hi, &kp_lo);

        double hi = c - kp_hi;
        double lo = ((c - hi) - kp_hi) - kp_lo;

        float f = (float)hi;
        double residual = (hi - (double)f) + lo;

        if (isfinite(k) && k != 0.0 && atan2f_gap_(f, residual) < ATAN2F_ERR) {
            double qh, ql;
            atan2f_exact_quot_(a, b, &qh, &ql);
            return atan2pif_accurate_(qh, ql, x < 0.0f, &atan2pif_half_dint_,
                                      hi, residual);
        }

        return dd_to_float_(hi, lo);
    }

    /* |x| >= |y|: acute angle atan(y/|x|)/pi, then the quadrant fold. */
    double k = 0.0; /* stays 0 when the acute value is exact */
    double ah, al = 0.0;

    if (a == b) {
        /* Equal magnitudes: exact quarter half-turns (or signed zero). */
        ah = b ? copysign(0.25, yd) : yd;
    } else {
        k = kernel_atanf_(yd / a);
        atan2pif_scale_(k, &ah, &al);
    }

    if (signbit(x)) {
        /* Second/third quadrant: copysign(1, y) - acute */
        double c = copysign(1.0, yd);
        double hi = c - ah;
        double lo = ((c - hi) - ah) - al;

        float f = (float)hi;
        double residual = (hi - (double)f) + lo;

        if (isfinite(k) && k != 0.0 && atan2f_gap_(f, residual) < ATAN2F_ERR) {
            double qh, ql;
            atan2f_exact_quot_(b, a, &qh, &ql);
            return atan2pif_accurate_(qh, ql, y < 0.0f, &atan2pif_one_dint_,
                                      hi, residual);
        }

        return dd_to_float_(hi, lo);
    }

    float f = (float)ah;
    double residual = (ah - (double)f) + al;

    if (isfinite(k) && k != 0.0 && f != 0.0f && atan2f_gap_(f, residual) < ATAN2F_ERR) {
        double qh, ql;
        atan2f_exact_quot_(b, a, &qh, &ql);
        return atan2pif_accurate_(qh, ql, y < 0.0f, NULL, ah, residual);
    }

    return dd_to_float_(ah, al);
}

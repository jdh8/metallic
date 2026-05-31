#ifndef METALLIC_KERNEL_GAMMA_FLOAT_H
#define METALLIC_KERNEL_GAMMA_FLOAT_H

/* Double-double machinery for a correctly-rounded tgammaf.
 *
 * tgammaf computes Gamma in double then rounds to float.  For float inputs the
 * double intermediate never overflows before the float result does (tgammaf
 * overflows at x ~ 35.04, far below DBL_MAX), so the whole task is to make the
 * double value accurate enough -- and to round it to float without a double
 * round -- so that every one of the 2^32 results is correctly rounded.
 *
 * The pieces below carry a hi+lo (double-double) pair through the algebraic
 * steps that feed the final float (the Lanczos rational sum, the exp2 scaling,
 * the final product, and the reflection division), then round the pair to the
 * nearest float with the residual breaking ties so the wide -> double -> float
 * path cannot double-round on a midpoint.
 *
 * The EFT primitives mirror the file-local mul_/add_ in src/math/double/fma.c;
 * fma() is a true correctly-rounded fused op on this target (no hardware FMA,
 * FP_FAST_FMA undefined), so it is used for the two-product residual. */

#include "../../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Two-sum: y = round(a+b), *r = exact error.  Knuth, no ordering required. */
static double gamma_twosum_(double a, double b, double r[static 1])
{
    double y = a + b;
    double s = y - a;

    *r = s - y + a + (b - s);
    return y;
}

/* Two-product: y = round(a*b), *r = exact error (via the true fused fma). */
static double gamma_twoprod_(double a, double b, double r[static 1])
{
    double y = a * b;

    *r = fma(a, b, -y);
    return y;
}

/* (a_hi + a_lo) * (b_hi + b_lo) -> (hi, *lo), double-double product. */
static double gamma_mul_dd_(double a_hi, double a_lo, double b_hi, double b_lo, double lo[static 1])
{
    double p_lo;
    double p_hi = gamma_twoprod_(a_hi, b_hi, &p_lo);

    p_lo += a_hi * b_lo + a_lo * b_hi;

    double hi = p_hi + p_lo;
    *lo = p_hi - hi + p_lo;
    return hi;
}

/* a / (b_hi + b_lo) -> (hi, *lo), double-double quotient of a scalar numerator
 * by a double-double denominator.  One Newton refinement on the residual. */
static double gamma_div_dd_(double a, double b_hi, double b_lo, double lo[static 1])
{
    double q = a / b_hi;

    /* residual = a - q*(b_hi + b_lo), computed with a fused product so the
     * leading term is exact. */
    double r_lo;
    double r_hi = gamma_twoprod_(q, b_hi, &r_lo);
    double residual = a - r_hi - r_lo - q * b_lo;

    double corr = residual / b_hi;

    double hi = q + corr;
    *lo = q - hi + corr;
    return hi;
}

/* Round a double-double (hi, lo) to the nearest float, breaking ties by the
 * residual so the wide -> double -> float path cannot double-round.
 *
 * The only hazard is when s = RN(hi + lo) lands exactly on a float midpoint:
 * the cast then ties-to-even, but the true value hi + lo lies on one definite
 * side of the midpoint.  Detect the midpoint magnitude-independently -- the
 * float ULP varies with the exponent, so a fixed bit mask will not do -- by
 * comparing s to the exact midpoint of its two surrounding floats, and resolve
 * the tie toward the residual e.  For every non-midpoint s (the common case)
 * the plain cast already rounds correctly. */
static float gamma_to_float_odd_(double hi, double lo)
{
    double e;
    double s = gamma_twosum_(hi, lo, &e);
    float rn = (float)s;

    if (e) {
        double ds = rn;

        if (ds != s) {
            /* Bracket s strictly between the floats a < s < b. */
            float a = ds < s ? rn : nextafterf(rn, -(float)INFINITY);
            float b = ds < s ? nextafterf(rn, (float)INFINITY) : rn;

            if (s == 0.5 * ((double)a + (double)b))
                return e > 0 ? b : a;
        }
    }

    return rn;
}

/* Horner over 8 double-double coefficients in the double-double variable u. */
static double gamma_poly8_dd_(const double *chi, const double *clo, double uhi, double ulo, double lo[static 1])
{
    double phi = chi[7], plo = clo[7];

    for (int i = 6; i >= 0; i--) {
        double t;
        phi = gamma_mul_dd_(phi, plo, uhi, ulo, &t);
        plo = t;
        phi = gamma_twosum_(phi, chi[i], &t);
        plo += t + clo[i];
    }

    *lo = plo;
    return phi;
}

/* sin(pi t)/(pi t) coefficients a_k = (-1)^k pi^(2k+1)/(2k+1)!, split to dd:
 * sin(pi t) = t * sum_k a_k t^(2k) for |t| <= 1/4 (t^2 <= 1/16). */
static const double gamma_sinpi_a_hi_[8] = {
     3.141592653589793,     -5.16771278004997,      2.550164039877345,
    -0.5992645293207921,     0.08214588661112823,  -0.007370430945714350,
     0.0004663028057676932, -2.1915353447830217e-05,
};
static const double gamma_sinpi_a_lo_[8] = {
     1.2246467991473532e-16, 2.2241363292326247e-16, -7.864671896773851e-17,
    -2.7064172561139924e-17, 2.7591435558369737e-18, -5.6884907021238545e-19,
     2.052498891805865e-20,  4.916628064464509e-22,
};
/* cos(pi u) coefficients b_k = (-1)^k pi^(2k)/(2k)!, split to dd. */
static const double gamma_cospi_b_hi_[8] = {
     1.0,                   -4.934802200544679,      4.058712126416768,
    -1.3352627688545895,     0.23533063035889312,   -0.025806891390014054,
     0.001929574309403922,  -0.0001046381049248457,
};
static const double gamma_cospi_b_lo_[8] = {
     0.0,                   -3.1326477033326616e-16, 2.1888843707431657e-16,
     2.557591896698939e-17, -1.0167242967024417e-17, -1.3344439210430054e-18,
    -7.851331287025183e-20,  1.1473530558267206e-21,
};

/* sin(pi x) for finite x as a double-double.  The gamma reflection needs the
 * sine to more than the ~24 bits a float sinpi delivers, or it caps the result
 * at ~1 binary32 ulp.  Reduce x = n + f with n = rint(x), |f| <= 1/2 (exact for
 * the |x| < 2^52 that reach here); sin(pi x) = (-1)^n sin(pi f).  On |f| <= 1/4
 * use sin(pi f) = (pi f) S(f^2); else sin(pi f) = cos(pi(1/2 - |f|)). */
static double gamma_sinpi_dd_(double x, double lo[static 1])
{
    double n = rint(x);
    double f = x - n;
    double a = fabs(f);
    double r, rl;

    if (a <= 0.25) {
        double u2lo, u2 = gamma_twoprod_(a, a, &u2lo);
        double slo, s = gamma_poly8_dd_(gamma_sinpi_a_hi_, gamma_sinpi_a_lo_, u2, u2lo, &slo);
        r = gamma_mul_dd_(s, slo, a, 0.0, &rl);
    } else {
        double vlo, v = gamma_twosum_(0.5, -a, &vlo);
        double v2lo, v2 = gamma_mul_dd_(v, vlo, v, vlo, &v2lo);
        r = gamma_poly8_dd_(gamma_cospi_b_hi_, gamma_cospi_b_lo_, v2, v2lo, &rl);
    }

    if (f < 0.0) { r = -r; rl = -rl; }
    if ((long long)n & 1LL) { r = -r; rl = -rl; }

    *lo = rl;
    return r;
}

#endif

/* C23 cospi: cos(pi*x), correctly rounded.
 *
 * Port of metallic-rs `cospi` (src/f64_/trig.rs).  The structure mirrors
 * sinpi -- the quarter-turn shift rides the grid index (+2048 before
 * halving), the mantissa stays unsigned (cosine is even), and the cross
 * term flips sign -- with the same sincosn table, residual polynomials, and
 * tier ladder.  The fast gate is CORE-MATH's residual-proportional z*2^-123
 * plus an absolute 2^-77 floor for the table pair's own ~2^-79 accuracy
 * near grid points, certified in trig.rs at >= 2x margin. */

#include "kernel/sincosn.h"
#include "kernel/trig.h"
#include "kernel/trigdint.h"

/* Taylor coefficients of (cos(pi*x) - 1)/x^2 for |x| <= 2^-12:
 * (-1)^(k+1) pi^(2k+2)/(2k+2)!. */
static const double cospi_small_c_[4] = {
    -0x1.3bd3cc9be45dep+2, 0x1.03c1f081b5ac4p+2, -0x1.55d3c7e3cbffap+0,
    0x1.e1f506891babbp-3,
};

/* Final tier -- sinpi's accurate tier rotated a quarter turn. */
static double cospi_accurate_(double x)
{
    double q = rint(2.0 * x);
    dint_t u = dint_from_f64_(2.0 * x - q);
    dint_t theta = dint_mul_(&u, &PIO2_DINT); /* pi(x - q/2) in [-pi/4, pi/4] */
    dint_t w;

    switch ((int64_t)q & 3) {
    case 0: w = cos_dint_(&theta); break;
    case 1: w = neg_dint_(sin_dint_(&theta)); break;
    case 2: w = neg_dint_(cos_dint_(&theta)); break;
    default: w = sin_dint_(&theta); break;
    }

    return dint_to_f64_(&w);
}

/* Middle tier -- sinpi's fast-kernel tier rotated a quarter turn. */
static int cospi_mid_(double x, double *out)
{
    double q = rint(2.0 * x);
    dd_t theta = trig_mul_f64_(TRIG_PIO2_DD, 2.0 * x - q);
    dd_t v;

    switch ((int64_t)q & 3) {
    case 0: v = trig_cos_kernel_fast_(theta); break;
    case 1: v = trig_neg_(trig_sin_kernel_fast_(theta)); break;
    case 2: v = trig_neg_(trig_cos_kernel_fast_(theta)); break;
    default: v = trig_sin_kernel_fast_(theta); break;
    }

    return trig_ziv_(v, out);
}

double cospi(double x)
{
    uint64_t ix = reinterpret(uint64_t, x);
    uint64_t ax = ix & (~0ULL >> 1);
    int64_t e = (int64_t)(ax >> 52);

    /* |x| >= 2^41 (every value is a multiple of 1/2048), +-inf, NaN. */
    if (e >= 1064) {
        if (e == 0x7ff)
            return ix << 12 == 0 ? (x - x) / (x - x) : x + x;

        if (e > 1075)
            return 1.0; /* |x| >= 2^53: all even integers */

        uint64_t m = (ix & (~0ULL >> 12)) | 1ULL << 52;
        /* |x|*2048 + a quarter turn, exact; evenness needs no sign fold. */
        uint64_t iq = (m << (e - 1064)) + 1024;

        if (!(iq & 2047))
            return 0.0; /* half-integer x: +0 always */

        exptab_sum_ sg, cg;
        sincosn_((int64_t)iq, &sg, &cg);
        return sg.hi + sg.lo; /* exact grid point: the table is the answer */
    }

    /* Small band |x| <= 2^-12: even Taylor around 1. */
    if (ax <= 0x3f30000000000000) {
        /* cos(pi*x) rounds to 1 for (pi*x)^2/2 <= 2^-54 (the expression
         * keeps CORE-MATH's shape; round-to-nearest folds it to 1.0). */
        if (ax <= 0x3e2ccf6429be6621)
            return 1.0 - 0x1p-55;

        double x2 = x * x;
        double x4 = x2 * x2;
        double eps = x2 * 0x1.38p-47;
        double p = x2 * ((cospi_small_c_[0] + x2 * cospi_small_c_[1])
            + x4 * (cospi_small_c_[2] + x2 * cospi_small_c_[3]));
        double lb = (p - eps) + 1.0;
        double ub = (p + eps) + 1.0;

        if (lb == ub)
            return lb;

        return cospi_accurate_(x);
    }

    /* Main band 2^-12 < |x| < 2^41 (e in [1011, 1063]): integer reduction. */
    uint64_t m = (ix & (~0ULL >> 12)) | 1ULL << 52;
    int64_t si = e - 1011;

    if (si >= 0 && m << si == 1ULL << 63)
        return 0.0; /* half-integer x: +0 always */

    int64_t s = 1063 - e; /* 0..=52 */
    uint64_t iq = ((((m >> s) + 2048) & 8191) + 1) >> 1;
    int64_t k = (int64_t)(m << (e - 1000));
    double z = (double)k;
    double z2 = z * z;
    double fs = z2 * (z2 * sincosn_zs_[2] + sincosn_zs_[1]) + sincosn_zs_[0];
    double fc = z2 * sincosn_zc_[1] + sincosn_zc_[0];
    exptab_sum_ sg, cg;
    sincosn_((int64_t)iq, &sg, &cg);
    double er = fabs(z) * 0x1p-123 + 0x1p-77;
    double r = cg.hi * (z * fs) + (sg.lo + sg.hi * (z2 * fc));
    double lb = (r - er) + sg.hi;
    double ub = (r + er) + sg.hi;

    if (lb == ub)
        return lb;

    double mid;
    if (cospi_mid_(x, &mid))
        return mid;

    return cospi_accurate_(x);
}

#include "dd.h"
#include "../double/kernel/exptab.h"
#include "../double/shift.h"
#include <complex.h>
#include <float.h>
#include <math.h>

int __rem_pio2(double x, double y[static 2]);

/* cos(r) and sin(r) as double-double Taylor series on the reduced |r| <= pi/4.
 * cos = sum c[k] u^k (u = r^2); sin = r * sum s[k] u^k.  Truncation < 2^-113. */
static const sum_t cis_cos_c_[16] = {
    { 1.0, 0.0 },
    { -0.5, 0.0 },
    { 0.041666666666666664, 2.3129646346357427e-18 },
    { -0.001388888888888889, 5.300543954373577e-20 },
    { 2.48015873015873e-05, 2.1511947866775882e-23 },
    { -2.755731922398589e-07, -2.3767714622250297e-23 },
    { 2.08767569878681e-09, -1.20734505911326e-25 },
    { -1.1470745597729725e-11, -2.0655512752830745e-28 },
    { 4.779477332387385e-14, 4.399205485834081e-31 },
    { -1.5619206968586225e-16, -1.1910679660273754e-32 },
    { 4.110317623312165e-19, 1.4412973378659527e-36 },
    { -8.896791392450574e-22, 7.911402614872376e-38 },
    { 1.6117375710961184e-24, -3.6846573564509766e-41 },
    { -2.4795962632247976e-27, 1.2953730964765229e-43 },
    { 3.279889237069838e-30, 1.5117542744029879e-46 },
    { -3.7699876288159054e-33, -2.5870347832750324e-49 },
};
static const sum_t cis_sin_c_[15] = {
    { 1.0, 0.0 },
    { -0.16666666666666666, -9.25185853854297e-18 },
    { 0.008333333333333333, 1.1564823173178714e-19 },
    { -0.0001984126984126984, -1.7209558293420705e-22 },
    { 2.7557319223985893e-06, -1.858393274046472e-22 },
    { -2.505210838544172e-08, 1.448814070935912e-24 },
    { 1.6059043836821613e-10, 1.2585294588752098e-26 },
    { -7.647163731819816e-13, -7.03872877733453e-30 },
    { 2.8114572543455206e-15, 1.6508842730861433e-31 },
    { -8.22063524662433e-18, -2.2141894119604265e-34 },
    { 1.9572941063391263e-20, -1.3643503830087908e-36 },
    { -3.868170170630684e-23, 8.843177655482344e-40 },
    { 6.446950284384474e-26, -1.9330404233703465e-42 },
    { -9.183689863795546e-29, -1.4303150396787322e-45 },
    { 1.1309962886447716e-31, 1.0498015412959506e-47 },
};

/* cos(y) and sin(y) as double-doubles. */
static void cisf_dd_(float y, sum_t *cosy, sum_t *siny)
{
    double v[2];
    unsigned q = (unsigned)__rem_pio2((double)y, v);
    sum_t r = dd_2sum_(v[0], v[1]);
    sum_t u = dd_mul_dd_(r, r);
    sum_t co = poly_dd_(u, cis_cos_c_, 16);
    sum_t si = dd_mul_dd_(r, poly_dd_(u, cis_sin_c_, 15));
    sum_t nco = { -co.hi, -co.lo };
    sum_t nsi = { -si.hi, -si.lo };

    switch (q & 3) {
        case 1: *cosy = nsi; *siny = co;  return;
        case 2: *cosy = nco; *siny = nsi; return;
        case 3: *cosy = si;  *siny = nco; return;
        default: *cosy = co; *siny = si;  return;
    }
}

/* exp(x) = 2^*q * (hi + lo) for finite x in [-104, 88.8]; q stays int64-safe. */
static sum_t expf_mant_dd_(double x, int64_t *qout)
{
    double scaled = rint(x * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;
    int j = (int)(n & (EXPTAB_N - 1));
    int64_t q = n >> 7;

    double a = x - scaled * exptab_ln2_over_n_hi_;
    exptab_sum_ blo = exptab_prod_(scaled, exptab_ln2_over_n_lo_);
    exptab_sum_ r = exptab_twosum_(a, -blo.hi);
    r = exptab_fast2sum_(r.hi, r.lo - blo.lo - scaled * exptab_ln2_over_n_lo2_);

    exptab_sum_ p = exptab_mantissa_(j, &q, r);
    *qout = q;
    return (sum_t){ p.hi, p.lo };
}

/* Correctly-rounded exp(x) as a float (the y == 0 real part). */
static float expf_cr_(float x)
{
    if (isnan(x))
        return x;
    if (x > 88.8f)
        return INFINITY;
    if (x < -104.0f)
        return 0;

    int64_t q;
    sum_t p = expf_mant_dd_((double)x, &q);
    p.hi = shift_(p.hi, q);
    p.lo = shift_(p.lo, q);
    return round_dd_general_(p);
}

float _Complex cexpf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    if (y == 0)
        return CMPLXF(expf_cr_(x), y);

    if (isinf(y) || isnan(y)) {
        if (x == INFINITY)
            return CMPLXF(x, y - y);
        if (x == -INFINITY)
            return 0;
        return CMPLXF(NAN, NAN);
    }

    /* finite y != 0 */
    sum_t c, s;
    cisf_dd_(y, &c, &s);

    /* exp(x) as a plain magnitude for the non-finite / out-of-range x cases.
     * The CR path scales the *product* exp(x)*cos(y), so it stays valid while
     * exp(x) overflows float yet the product is finite; it only breaks once
     * exp(x) overflows double (x > 709.78, where the n = round(x*N/ln2)
     * reduction would overflow int64) or underflows to 0 (x < -104, where the
     * product is below 2^-150 and rounds to zero regardless of cos).  Those, and
     * +-inf / NaN, reduce to r * cis -- the correctly-signed infinities and
     * zeros C / Annex G want. */
    if (isnan(x) || (double)x > 0x1.62e42fefa39efp+9 || x <= -104.0f) {
        double r = isnan(x) ? (double)NAN : (x > 0 ? INFINITY : 0.0);
        return CMPLXF((float)(r * c.hi), (float)(r * s.hi));
    }

    int64_t q;
    sum_t p = expf_mant_dd_((double)x, &q);
    sum_t re = dd_mul_dd_(p, c);
    sum_t im = dd_mul_dd_(p, s);
    re.hi = shift_(re.hi, q);
    re.lo = shift_(re.lo, q);
    im.hi = shift_(im.hi, q);
    im.lo = shift_(im.lo, q);
    return CMPLXF(round_dd_signed_(re), round_dd_signed_(im));
}

#ifndef METALLIC_KERNEL_TANPITAB_H
#define METALLIC_KERNEL_TANPITAB_H

/* Grid table and reconstruction quotient of the C23 tanpi, ported from
 * metallic-rs (src/f64_/trig.rs: TANPI_T, TANPI_PH, TANPI_ZC,
 * tanpi_reconstruct; itself a port of CORE-MATH's cr_tanpi). */

#include "exptab.h"
#include "../../reinterpret.h"
#include <stdint.h>

/* pi as a double-double (trig.rs PI_DD). */
static const double tanpi_pi_hi_ = 0x1.921fb54442d18p+1;
static const double tanpi_pi_lo_ = 0x1.1a62633145c07p-53;

/* pi*2^-70 as a double-double -- the slope of the residual angle in tanpi's
 * fixed-point z scale (trig.rs TANPI_PH). */
static const double tanpi_ph_hi_ = 0x1.921fb54442d18p-69;
static const double tanpi_ph_lo_ = 0x1.1a62633145c07p-123;

/* tan(pi*k/64) as (high, low) double-doubles, k = 0..=31 -- CORE-MATH
 * tanpi's T; the pole-adjacent cell (k = 32) collapses to -1/t instead. */
static const double tanpi_t_[32][2] = {
    { 0x0.0p+0, 0x0.0p+0 },
    { 0x1.927278a3b1162p-5, 0x1.ada13ceebab9dp-64 },
    { 0x1.936bb8c5b2da2p-4, 0x1.87d054f689d7ap-58 },
    { 0x1.2fcac73a60640p-3, 0x1.2cfb5a746f62cp-58 },
    { 0x1.975f5e0553158p-3, 0x1.ef5d367441946p-61 },
    { 0x1.007fa758626aep-2, 0x1.5a6d6c3c8b6a7p-57 },
    { 0x1.36a08355c63dcp-2, -0x1.d704d1bfdb6e8p-57 },
    { 0x1.6e649f7d78649p-2, 0x1.78e945dc3913cp-57 },
    { 0x1.a827999fcef32p-2, 0x1.08b2fb1366ea9p-56 },
    { 0x1.e450e0d273e7ap-2, 0x1.41522f15e53c5p-56 },
    { 0x1.11ab7190834ecp-1, -0x1.e564bcd1e635dp-55 },
    { 0x1.32e1889047ffdp-1, -0x1.10b4421e6a4f8p-55 },
    { 0x1.561b82ab7f990p-1, 0x1.7a8c52172b675p-55 },
    { 0x1.7bb99ed2990cfp-1, -0x1.aa7538e44e996p-55 },
    { 0x1.a43002ae42850p-1, -0x1.a6db80fe796fep-56 },
    { 0x1.d00cbc7384d2ep-1, 0x1.8dcad85e60fbep-56 },
    { 0x1.0000000000000p+0, 0x0.0p+0 },
    { 0x1.1a73d55278c4bp+0, 0x1.946cc0b66979fp-54 },
    { 0x1.37efd8d87607ep+0, -0x1.87e5ad9735569p-56 },
    { 0x1.592d11142fa55p+0, 0x1.6a085e3bc3af6p-55 },
    { 0x1.7f218e25a7461p+0, 0x1.419fa6954928fp-54 },
    { 0x1.ab1c35d8a74eap+0, -0x1.b6fb77846d763p-56 },
    { 0x1.def13b73c1406p+0, 0x1.0fb3e75c7098ep-55 },
    { 0x1.0ea21d716fbf7p+1, 0x1.7ce6cb463c972p-54 },
    { 0x1.3504f333f9de6p+1, 0x1.21165f626cdd5p-53 },
    { 0x1.65bc6cc825147p+1, 0x1.aca947bfb1dccp-56 },
    { 0x1.a5f59e90600ddp+1, -0x1.b7a14d0d691bep-54 },
    { 0x1.ff01305ecd8dcp+1, -0x1.89fcd637fbf3bp-55 },
    { 0x1.41bfee2424771p+2, 0x1.10706fed37f0ep-55 },
    { 0x1.af73f4ca3310fp+2, -0x1.ae397239c5a0dp-53 },
    { 0x1.44e6c595afdccp+3, 0x1.351daea79411dp-51 },
    { 0x1.45affed201b55p+4, -0x1.b6e6b4de0cd24p-56 },
};

/* (tan(theta) - theta)/z^3 residual Taylor coefficients in the 2^-63*2^-7
 * fixed-point z scale: with T = pi*2^-70 these are T^3/3, 2T^5/15,
 * 17T^7/315, 62T^9/2835, 1382T^11/155925 exactly (trig.rs TANPI_ZC; the
 * deg-11 term is load-bearing near the pole-adjacent cells). */
static const double tanpi_zc_[5] = {
    0x1.4abbce625be53p-207, 0x1.466bc6775aae2p-345, 0x1.45fff9b48e95ep-483,
    0x1.45f472e3af046p-621, 0x1.45f32f256a92ep-759,
};

/* Reconstruct tan(pi*(a + theta)) = (n + t)/(1 - n*t) from the grid tangent
 * n = +-tanpi_t_[iq] and the residual tangent t -- CORE-MATH's fused
 * double-double divide.  iq == 32 is the pole-adjacent cell, where the
 * identity collapses to -1/t.
 *
 * This is the main-band evaluator (hot), so the Rust original's fma sites
 * are reproduced bit-for-bit without fma(): the a*b - a*b residuals are
 * Dekker two-prods, and the 1 -+ p shapes (p ~ +-1 from a reciprocal seed)
 * split as Sterbenz-exact 1 -+ p.hi followed by the Dekker low word. */
static inline exptab_sum_ tanpi_reconstruct_(exptab_sum_ t, int64_t iq, int neg_n)
{
    if (iq == 32) {
        double ith = -1.0 / t.hi;
        exptab_sum_ p = exptab_prod_(ith, t.hi);
        double e = (1.0 + p.hi) + p.lo; /* == fma(ith, t.hi, 1.0) */
        double tl = (e + t.lo * ith) * ith;
        return (exptab_sum_){ ith, tl };
    }

    uint64_t sgn = neg_n ? 1ULL << 63 : 0;
    uint64_t nh_bits = reinterpret(uint64_t, tanpi_t_[iq][0]) ^ sgn;
    uint64_t nl_bits = reinterpret(uint64_t, tanpi_t_[iq][1]) ^ sgn;
    double nh = reinterpret(double, nh_bits);
    double nl = reinterpret(double, nl_bits);

    /* m = 1 - n*t (dd), num = n + t (dd), result = num/m with one refined
     * reciprocal -- CORE-MATH's operation order, kept for the certified eps. */
    exptab_sum_ m0 = exptab_prod_(nh, t.hi);
    double ml0 = m0.lo + (nh * t.lo + nl * t.hi);
    exptab_sum_ m = exptab_fast2sum_(1.0, -m0.hi);
    double ml = m.lo - ml0;
    exptab_sum_ n = exptab_fast2sum_(nh, t.hi);
    double nl2 = nl + (n.lo + t.lo);
    double imh = 1.0 / m.hi;
    exptab_sum_ q = exptab_prod_(n.hi, imh);
    exptab_sum_ w = exptab_prod_(m.hi, imh);
    double resid = (1.0 - w.hi) - w.lo; /* == fma(-m.hi, imh, 1.0) */
    double tl = q.lo + (nl2 + n.hi * (resid - ml * imh)) * imh;
    return (exptab_sum_){ q.hi, tl };
}

#endif

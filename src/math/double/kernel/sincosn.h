#ifndef METALLIC_KERNEL_SINCOSN_H
#define METALLIC_KERNEL_SINCOSN_H

/* Two-level (sin, cos) grid at s*pi/2048 shared by the C23 sinpi/cospi pair,
 * ported from metallic-rs (src/f64_/trig.rs: sincosn, SINCOSN_SN/SM/CM,
 * SINPI_ZS/ZC; itself a port of CORE-MATH sinpi's sincosn).  The <= 25-bit
 * table highs keep the angle-addition cross products exact in plain doubles,
 * so no FMA (which WASM lacks) is needed on this hot path. */

#include "exptab.h"
#include "../../reinterpret.h"
#include <stdint.h>

/* pi as a double-double (trig.rs PI_DD). */
static const double sincosn_pi_hi_ = 0x1.921fb54442d18p+1;
static const double sincosn_pi_lo_ = 0x1.1a62633145c07p-53;

/* sin(pi i/64), i = 0..=32, as (short-high, low) -- CORE-MATH sinpi Sn;
 * cosine reads the same table by complement (32 - i). */
static const double sincosn_sn_[33][2] = {
    { 0x0.0p+0, 0x0.0p+0 },
    { 0x1.91f6600000000p-5, -0x1.de44fd832257ap-30 },
    { 0x1.917a6c0000000p-4, -0x1.eb25ea0f138c7p-31 },
    { 0x1.2c81060000000p-3, 0x1.d1cc27444c003p-28 },
    { 0x1.8f8b840000000p-3, -0x1.cb2cfaa4da337p-30 },
    { 0x1.f19f980000000p-3, -0x1.37a839542deefp-29 },
    { 0x1.2940630000000p-2, -0x1.2a60fa574a369p-30 },
    { 0x1.58f9a70000000p-2, 0x1.6ac7f73f84090p-28 },
    { 0x1.87de2a0000000p-2, 0x1.abaa58b469891p-28 },
    { 0x1.b5d1010000000p-2, -0x1.87a8cff5264eap-28 },
    { 0x1.e2b5d40000000p-2, -0x1.fe4271387c9dcp-28 },
    { 0x1.0738798000000p-1, 0x1.22ffed9697fafp-29 },
    { 0x1.1c73b38000000p-1, 0x1.ae68c86c9774ap-29 },
    { 0x1.30ff800000000p-1, -0x1.8f47e58f7e631p-28 },
    { 0x1.44cf328000000p-1, -0x1.7b7114f3fc4afp-28 },
    { 0x1.57d6938000000p-1, -0x1.b989b02eae413p-28 },
    { 0x1.6a09e68000000p-1, -0x1.80c4336f74d05p-29 },
    { 0x1.7b5df20000000p-1, 0x1.3557d76f0ac85p-28 },
    { 0x1.8bc8068000000p-1, 0x1.8a8ba05a743dap-28 },
    { 0x1.9b3e048000000p-1, -0x1.8f17e98771434p-34 },
    { 0x1.a9b6628000000p-1, 0x1.0ea1a3033ec62p-29 },
    { 0x1.b728348000000p-1, -0x1.7348e1378d3e6p-28 },
    { 0x1.c38b2f0000000p-1, 0x1.80bdb0d23e9d1p-29 },
    { 0x1.ced7af8000000p-1, -0x1.e19c46879edafp-28 },
    { 0x1.d906bd0000000p-1, -0x1.9ae573aea067cp-30 },
    { 0x1.e212108000000p-1, -0x1.84bc8da0298eep-28 },
    { 0x1.e9f4158000000p-1, -0x1.39d225a27d387p-29 },
    { 0x1.f0a7ef8000000p-1, 0x1.c9186b952c7aep-28 },
    { 0x1.f6297d0000000p-1, -0x1.1469faa77a357p-34 },
    { 0x1.fa75580000000p-1, -0x1.eeb5d2bd05465p-30 },
    { 0x1.fd88da0000000p-1, 0x1.e89292cf04139p-28 },
    { 0x1.ff621e0000000p-1, 0x1.bcb6bef1d421fp-28 },
    { 0x1.0000000000000p+0, 0x0.0p+0 },
};

/* sin(pi j/2048), j = 0..=31, short-high -- CORE-MATH sinpi Sm. */
static const double sincosn_sm_[32][2] = {
    { 0x0.0p+0, 0x0.0p+0 },
    { 0x1.9220000000000p-10, -0x1.54466e349ee53p-28 },
    { 0x1.921f800000000p-9, 0x1.7d99497495d20p-30 },
    { 0x1.2d97800000000p-8, 0x1.17ccb5e27cb43p-31 },
    { 0x1.921f000000000p-8, 0x1.fcce00e23572dp-29 },
    { 0x1.f6a6400000000p-8, 0x1.f9a2a3c5885cfp-28 },
    { 0x1.2d96c00000000p-7, -0x1.e35ed1fa23d22p-28 },
    { 0x1.5fda000000000p-7, 0x1.bd602f04014c8p-30 },
    { 0x1.921d200000000p-7, -0x1.909c3dccf0e28p-34 },
    { 0x1.c460000000000p-7, -0x1.e1b7526ebf9f2p-31 },
    { 0x1.f6a2a00000000p-7, -0x1.2a8cd06af94a2p-28 },
    { 0x1.1472700000000p-6, 0x1.b5ae26618769ep-31 },
    { 0x1.2d93700000000p-6, -0x1.1073c40b25037p-28 },
    { 0x1.46b4400000000p-6, -0x1.f80c5f9200607p-28 },
    { 0x1.5fd4d00000000p-6, 0x1.0fd5912ef3f57p-29 },
    { 0x1.78f5300000000p-6, 0x1.77727c10d8ca5p-28 },
    { 0x1.9215600000000p-6, -0x1.0b933040d8eb2p-31 },
    { 0x1.ab35500000000p-6, -0x1.3abec0d92ae48p-28 },
    { 0x1.c454f00000000p-6, 0x1.3394ec7229c28p-28 },
    { 0x1.dd74600000000p-6, -0x1.ce6d5319d653ep-28 },
    { 0x1.f693700000000p-6, 0x1.8e8e7807f600bp-29 },
    { 0x1.07d9200000000p-5, -0x1.9eea00d71246dp-35 },
    { 0x1.1468600000000p-5, -0x1.25e9f40a5121ap-28 },
    { 0x1.20f7700000000p-5, 0x1.9d6238d09f231p-30 },
    { 0x1.2d86580000000p-5, -0x1.4d75465d2f213p-30 },
    { 0x1.3a15100000000p-5, -0x1.37bca0781d73ap-30 },
    { 0x1.46a3980000000p-5, -0x1.0079e86eec954p-29 },
    { 0x1.5331f00000000p-5, -0x1.e222f8a75de87p-28 },
    { 0x1.5fc0100000000p-5, -0x1.6b7995e4bb32dp-28 },
    { 0x1.6c4df80000000p-5, -0x1.41523e139c56bp-32 },
    { 0x1.78dba80000000p-5, 0x1.2c3a342eb5f11p-28 },
    { 0x1.8569200000000p-5, 0x1.5da89e0b235c0p-28 },
};

/* cos(pi j/2048), j = 0..=31, short-high -- CORE-MATH sinpi Cm. */
static const double sincosn_cm_[32][2] = {
    { 0x1.0000000000000p+0, 0x0.0p+0 },
    { 0x1.ffffd88000000p-1, 0x1.61bb991af64f1p-31 },
    { 0x1.ffff620000000p-1, 0x1.621d01d2a6063p-29 },
    { 0x1.fffe9c8000000p-1, 0x1.8f17465de1773p-28 },
    { 0x1.fffd888000000p-1, -0x1.38bab6d94c71dp-28 },
    { 0x1.fffc250000000p-1, 0x1.6bb5dd7625bcdp-30 },
    { 0x1.fffa730000000p-1, -0x1.b439d8a459600p-28 },
    { 0x1.fff8718000000p-1, 0x1.37ce2eec7251bp-29 },
    { 0x1.fff6218000000p-1, -0x1.646d24a88970ep-29 },
    { 0x1.fff3828000000p-1, -0x1.9bb848bdb041ep-28 },
    { 0x1.fff0940000000p-1, 0x1.e29de85718cc2p-28 },
    { 0x1.ffed570000000p-1, 0x1.cc695b5e89e49p-28 },
    { 0x1.ffe9cb8000000p-1, -0x1.da572f6a4bccap-28 },
    { 0x1.ffe5f08000000p-1, -0x1.0d43929b71f74p-28 },
    { 0x1.ffe1c68000000p-1, 0x1.c32ddd89aa147p-31 },
    { 0x1.ffdd4d8000000p-1, 0x1.fbc7a9242ccf3p-28 },
    { 0x1.ffd8860000000p-1, 0x1.099a19765595dp-30 },
    { 0x1.ffd36f8000000p-1, -0x1.dbaff3c93cdc4p-29 },
    { 0x1.ffce0a0000000p-1, -0x1.8eacc3af7ac55p-28 },
    { 0x1.ffc8558000000p-1, -0x1.996f62d2df41dp-28 },
    { 0x1.ffc2520000000p-1, -0x1.071603e8582dfp-28 },
    { 0x1.ffbbff8000000p-1, 0x1.7e5454b8225e9p-31 },
    { 0x1.ffb55e8000000p-1, -0x1.ed0128d24b027p-28 },
    { 0x1.ffae6e0000000p-1, 0x1.5569984bd1a7ap-29 },
    { 0x1.ffa72f0000000p-1, -0x1.08a362d33736dp-37 },
    { 0x1.ff9fa10000000p-1, 0x1.a441ba9901fd5p-32 },
    { 0x1.ff97c40000000p-1, 0x1.04600a0a95596p-28 },
    { 0x1.ff8f988000000p-1, -0x1.387d3a589fa3dp-28 },
    { 0x1.ff871d8000000p-1, 0x1.6dc0ef98b1c67p-28 },
    { 0x1.ff7e540000000p-1, 0x1.01907c4c59658p-28 },
    { 0x1.ff753b8000000p-1, 0x1.8dc8b1e83ccffp-28 },
    { 0x1.ff6bd48000000p-1, -0x1.c4bbb2c348040p-29 },
};

/* sin(theta)/z and (cos(theta) - 1)/z^2 residual Taylor coefficients in the
 * fixed-point scale z = residual*2^63 of the grid unit: the reductions keep
 * the residual as the low bits of x*2^75, so with S = pi*2^-75 these are
 * S, -S^3/3!, S^5/5! and -S^2/2!, S^4/4! exactly (trig.rs SINPI_ZS/ZC). */
static const double sincosn_zs_[3] = {
    0x1.921fb54442d18p-74, -0x1.4abbce625be53p-223, 0x1.466bc6775aae2p-374,
};
static const double sincosn_zc_[2] = {
    -0x1.3bd3cc9be45dep-148, 0x1.03c1f081b5ac4p-298,
};

/* (sin, cos) at the grid angle s*pi/2048 as loose double-doubles; s is taken
 * modulo 4096 through two's-complement bit logic (works for negative s
 * unchanged): the low ten bits index within a quadrant (reflected in odd
 * quadrants), split into the pi/64 level (sincosn_sn_, cosine by complement)
 * and the pi/2048 level (sincosn_sm_/sincosn_cm_); one Fast2Sum per component
 * renormalizes the pair.  Bits 11/12 of s supply the half-turn signs, applied
 * branchlessly by XOR on the sign bit (a 50/50 data-dependent branch would
 * mispredict on random input). */
static inline void sincosn_(int64_t s, exptab_sum_ *sq, exptab_sum_ *cq)
{
    uint64_t us = (uint64_t)s;
    uint64_t j0 = us & 0x3ff;
    /* Odd quadrant: reflect j -> 1024 - j.  All-ones-mask form: the Rust
     * original's (it << 10) on it in {0, -1} would be UB in C. */
    uint64_t it = -((us >> 10) & 1);
    uint64_t j = (~it & j0) | (it & (1024 - j0));
    uint64_t is = j >> 5;
    uint64_t jm = j & 0x1f;
    uint64_t ssgn = ((us >> 11) & 1) << 63;
    uint64_t csgn = (((us + 1024) >> 11) & 1) << 63;

    double sbh = sincosn_sn_[is][0], sbl = sincosn_sn_[is][1];
    double cbh = sincosn_sn_[32 - is][0], cbl = sincosn_sn_[32 - is][1];
    double smh = sincosn_sm_[jm][0], sml = sincosn_sm_[jm][1];
    double cmh = sincosn_cm_[jm][0], cml = sincosn_cm_[jm][1];

    /* Angle addition: the <= 25-bit high*high products are exact, and the
     * certified budget assumes each term rounds on its own (no FMA). */
    double sbf = sbh + sbl;
    double cbf = cbh + cbl;
    double ch = cbh * cmh - sbh * smh;
    double cl = cmh * cbl - smh * sbl + (cbf * cml - sbf * sml);
    double sh = sbh * cmh + cbh * smh;
    double sl = smh * cbl + cmh * sbl + (cbf * sml + sbf * cml);

    exptab_sum_ s2 = exptab_fast2sum_(sh, sl);
    exptab_sum_ c2 = exptab_fast2sum_(ch, cl);
    uint64_t s2hi = reinterpret(uint64_t, s2.hi) ^ ssgn;
    uint64_t s2lo = reinterpret(uint64_t, s2.lo) ^ ssgn;
    uint64_t c2hi = reinterpret(uint64_t, c2.hi) ^ csgn;
    uint64_t c2lo = reinterpret(uint64_t, c2.lo) ^ csgn;
    sq->hi = reinterpret(double, s2hi);
    sq->lo = reinterpret(double, s2lo);
    cq->hi = reinterpret(double, c2hi);
    cq->lo = reinterpret(double, c2lo);
}

#endif

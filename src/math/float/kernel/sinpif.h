#ifndef METALLIC_FLOAT_KERNEL_SINPIF_H
#define METALLIC_FLOAT_KERNEL_SINPIF_H

/* Shared tables for the correctly rounded sinpif/cospif (metallic-rs
 * f32_/trig.rs, CORE-MATH cr_sinpif/cr_cospif structure). */

/* sin(pi k/64) for the full circle, k = 0..127; signs are baked in, and
 * sinpif_s_[(k + 32) & 127] reads the cosine. */
static const double sinpif_s_[128] = {
    0x0.0p+0, 0x1.91f65f10dd814p-5, 0x1.917a6bc29b42cp-4, 0x1.2c8106e8e613ap-3,
    0x1.8f8b83c69a60bp-3, 0x1.f19f97b215f1bp-3, 0x1.294062ed59f06p-2, 0x1.58f9a75ab1fddp-2,
    0x1.87de2a6aea963p-2, 0x1.b5d1009e15cc0p-2, 0x1.e2b5d3806f63bp-2, 0x1.073879922ffeep-1,
    0x1.1c73b39ae68c8p-1, 0x1.30ff7fce17035p-1, 0x1.44cf325091dd6p-1, 0x1.57d69348ceca0p-1,
    0x1.6a09e667f3bcdp-1, 0x1.7b5df226aafafp-1, 0x1.8bc806b151741p-1, 0x1.9b3e047f38741p-1,
    0x1.a9b66290ea1a3p-1, 0x1.b728345196e3ep-1, 0x1.c38b2f180bdb1p-1, 0x1.ced7af43cc773p-1,
    0x1.d906bcf328d46p-1, 0x1.e212104f686e5p-1, 0x1.e9f4156c62ddap-1, 0x1.f0a7efb9230d7p-1,
    0x1.f6297cff75cb0p-1, 0x1.fa7557f08a517p-1, 0x1.fd88da3d12526p-1, 0x1.ff621e3796d7ep-1,
    0x1.0000000000000p+0, 0x1.ff621e3796d7ep-1, 0x1.fd88da3d12526p-1, 0x1.fa7557f08a517p-1,
    0x1.f6297cff75cb0p-1, 0x1.f0a7efb9230d7p-1, 0x1.e9f4156c62ddap-1, 0x1.e212104f686e5p-1,
    0x1.d906bcf328d46p-1, 0x1.ced7af43cc773p-1, 0x1.c38b2f180bdb1p-1, 0x1.b728345196e3ep-1,
    0x1.a9b66290ea1a3p-1, 0x1.9b3e047f38741p-1, 0x1.8bc806b151741p-1, 0x1.7b5df226aafafp-1,
    0x1.6a09e667f3bcdp-1, 0x1.57d69348ceca0p-1, 0x1.44cf325091dd6p-1, 0x1.30ff7fce17035p-1,
    0x1.1c73b39ae68c8p-1, 0x1.073879922ffeep-1, 0x1.e2b5d3806f63bp-2, 0x1.b5d1009e15cc0p-2,
    0x1.87de2a6aea963p-2, 0x1.58f9a75ab1fddp-2, 0x1.294062ed59f06p-2, 0x1.f19f97b215f1bp-3,
    0x1.8f8b83c69a60bp-3, 0x1.2c8106e8e613ap-3, 0x1.917a6bc29b42cp-4, 0x1.91f65f10dd814p-5,
    0x0.0p+0, -0x1.91f65f10dd814p-5, -0x1.917a6bc29b42cp-4, -0x1.2c8106e8e613ap-3,
    -0x1.8f8b83c69a60bp-3, -0x1.f19f97b215f1bp-3, -0x1.294062ed59f06p-2, -0x1.58f9a75ab1fddp-2,
    -0x1.87de2a6aea963p-2, -0x1.b5d1009e15cc0p-2, -0x1.e2b5d3806f63bp-2, -0x1.073879922ffeep-1,
    -0x1.1c73b39ae68c8p-1, -0x1.30ff7fce17035p-1, -0x1.44cf325091dd6p-1, -0x1.57d69348ceca0p-1,
    -0x1.6a09e667f3bcdp-1, -0x1.7b5df226aafafp-1, -0x1.8bc806b151741p-1, -0x1.9b3e047f38741p-1,
    -0x1.a9b66290ea1a3p-1, -0x1.b728345196e3ep-1, -0x1.c38b2f180bdb1p-1, -0x1.ced7af43cc773p-1,
    -0x1.d906bcf328d46p-1, -0x1.e212104f686e5p-1, -0x1.e9f4156c62ddap-1, -0x1.f0a7efb9230d7p-1,
    -0x1.f6297cff75cb0p-1, -0x1.fa7557f08a517p-1, -0x1.fd88da3d12526p-1, -0x1.ff621e3796d7ep-1,
    -0x1.0000000000000p+0, -0x1.ff621e3796d7ep-1, -0x1.fd88da3d12526p-1, -0x1.fa7557f08a517p-1,
    -0x1.f6297cff75cb0p-1, -0x1.f0a7efb9230d7p-1, -0x1.e9f4156c62ddap-1, -0x1.e212104f686e5p-1,
    -0x1.d906bcf328d46p-1, -0x1.ced7af43cc773p-1, -0x1.c38b2f180bdb1p-1, -0x1.b728345196e3ep-1,
    -0x1.a9b66290ea1a3p-1, -0x1.9b3e047f38741p-1, -0x1.8bc806b151741p-1, -0x1.7b5df226aafafp-1,
    -0x1.6a09e667f3bcdp-1, -0x1.57d69348ceca0p-1, -0x1.44cf325091dd6p-1, -0x1.30ff7fce17035p-1,
    -0x1.1c73b39ae68c8p-1, -0x1.073879922ffeep-1, -0x1.e2b5d3806f63bp-2, -0x1.b5d1009e15cc0p-2,
    -0x1.87de2a6aea963p-2, -0x1.58f9a75ab1fddp-2, -0x1.294062ed59f06p-2, -0x1.f19f97b215f1bp-3,
    -0x1.8f8b83c69a60bp-3, -0x1.2c8106e8e613ap-3, -0x1.917a6bc29b42cp-4, -0x1.91f65f10dd814p-5,
};

/* sin(theta)/z residual Taylor coefficients in the fixed-point z scale: with
 * F = pi * 2^-38 (the 1/64-grid unit over the 2^32 fixed point) these are
 * F, -F^3/3!, F^5/5!, -F^7/7! exactly.  The deg-7 term matters: without it
 * the ~2^-44 tail flips the f32 rounding at |x| = 0x1.921fbp-6; with it the
 * tail sits at ~2^-58 and the exhaustive sweep is clean. */
static const double sinpif_sn_[4] = {
    0x1.921fb54442d18p-37, -0x1.4abbce625be53p-112, 0x1.466bc6775aae2p-189, -0x1.32d2cce62bd86p-267,
};

/* (cos(theta) - 1)/z^2 residual Taylor coefficients: -F^2/2!, F^4/4!, -F^6/6! */
static const double sinpif_cn_[3] = {
    -0x1.3bd3cc9be45dep-74, 0x1.03c1f081b5ac4p-150, -0x1.55d3c7e3cbffap-228,
};

#endif

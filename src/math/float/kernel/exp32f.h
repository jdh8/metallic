#ifndef METALLIC_FLOAT_KERNEL_EXP32F_H
#define METALLIC_FLOAT_KERNEL_EXP32F_H

/* Shared 1/32-step exponential grid for exp2m1f/exp10m1f (metallic-rs
 * f32_/exp.rs): with a = 32*log2(b)*x, m = round(a) = 32q + j, and
 * h = a - m in [-1/2, 1/2], b^x = 2^q * 2^(j/32) * 2^(h/32).  One exponent
 * injection on the table entry supplies 2^q * 2^(j/32); the degree-4 kernel
 * covers 2^(h/32). */

/* 2^(j/32), j = 0..31 */
static const double exp32f_t_[32] = {
    0x1.0000000000000p+0, 0x1.059b0d3158574p+0, 0x1.0b5586cf9890fp+0, 0x1.11301d0125b51p+0,
    0x1.172b83c7d517bp+0, 0x1.1d4873168b9aap+0, 0x1.2387a6e756238p+0, 0x1.29e9df51fdee1p+0,
    0x1.306fe0a31b715p+0, 0x1.371a7373aa9cbp+0, 0x1.3dea64c123422p+0, 0x1.44e086061892dp+0,
    0x1.4bfdad5362a27p+0, 0x1.5342b569d4f82p+0, 0x1.5ab07dd485429p+0, 0x1.6247eb03a5585p+0,
    0x1.6a09e667f3bcdp+0, 0x1.71f75e8ec5f74p+0, 0x1.7a11473eb0187p+0, 0x1.82589994cce13p+0,
    0x1.8ace5422aa0dbp+0, 0x1.93737b0cdc5e5p+0, 0x1.9c49182a3f090p+0, 0x1.a5503b23e255dp+0,
    0x1.ae89f995ad3adp+0, 0x1.b7f76f2fb5e47p+0, 0x1.c199bdd85529cp+0, 0x1.cb720dcef9069p+0,
    0x1.d5818dcfba487p+0, 0x1.dfc97337b9b5fp+0, 0x1.ea4afa2a490dap+0, 0x1.f50765b6e4540p+0,
};

/* 2^(h/32) for h in [-1/2, 1/2], degree 4. */
static const double exp32f_p_[5] = {
    0x1.0000000000000p+0, 0x1.62e42fef6d01ap-6, 0x1.ebfbdff8131c3p-13, 0x1.c6b167e571135p-20,
    0x1.3b2b1bee88b09p-27,
};

#endif

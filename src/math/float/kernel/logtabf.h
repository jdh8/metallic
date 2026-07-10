#ifndef METALLIC_KERNEL_LOGTABF_H
#define METALLIC_KERNEL_LOGTABF_H

/* Mantissa-indexed natural-log reduction shared by the float log family,
 * ported from CORE-MATH's cr_logf (https://core-math.gitlabpages.inria.fr/,
 * MIT license, copyright (c) 2023-2026 Alexei Sibidanov and Paul Zimmermann).
 *
 * For x = 2^e * m with m in [1, 2), j = round(64*(m - 1)) picks a reciprocal
 * logtabf_r_[j] ~ 1/(1 + j/64) rounded to 29 significand bits, so that
 * z = m*r[j] - 1 with |z| <~ 2^-7 is EXACT when m carries a float's 24-bit
 * significand (24 + 29 <= 53 bits).  Then
 *
 *     log(x) ~ e*ln2 + l[j] + z*(b[0] + z*(b[1] + z*b[2]))
 *
 * where l[j] = -log(r[j]) and l[0] carries a small negative offset making the
 * error one-sided: CORE-MATH proves the exact log lies in [r, r + LOGTABF_EPS]
 * for cr_logf's evaluation order, which gates the fast leg by two float
 * roundings.  Derived users must widen the gate on both sides: log2f/log10f
 * rescale the natural log (extra roundings ~2^-45); asinhf/acoshf/atanhf index
 * with a computed double's mantissa, where z = m*r[j] - 1 is inexact by
 * <= 2^-53 -- negligible against the ~2^-33 gate.  The exhaustive oracles
 * (the _cr drivers under test/oracle/math/float) pin every widened bound.
 */

#include "../../reinterpret.h"
#include <stdint.h>

/* 1/(1 + j/64) rounded to 29 significand bits, j = 0..64 */
static const double logtabf_r_[] = {
    0x1p+0, 0x1.f81f82p-1, 0x1.f07c1fp-1, 0x1.e9131acp-1,
    0x1.e1e1e1ep-1, 0x1.dae6077p-1, 0x1.d41d41dp-1, 0x1.cd85689p-1,
    0x1.c71c71cp-1, 0x1.c0e0704p-1, 0x1.bacf915p-1, 0x1.b4e81b5p-1,
    0x1.af286bdp-1, 0x1.a98ef6p-1, 0x1.a41a41ap-1, 0x1.9ec8e95p-1,
    0x1.999999ap-1, 0x1.948b0fdp-1, 0x1.8f9c19p-1, 0x1.8acb90fp-1,
    0x1.8618618p-1, 0x1.8181818p-1, 0x1.7d05f41p-1, 0x1.78a4c81p-1,
    0x1.745d174p-1, 0x1.702e05cp-1, 0x1.6c16c17p-1, 0x1.6816817p-1,
    0x1.642c859p-1, 0x1.605816p-1, 0x1.5c9882cp-1, 0x1.58ed231p-1,
    0x1.5555555p-1, 0x1.51d07ebp-1, 0x1.4e5e0a7p-1, 0x1.4afd6ap-1,
    0x1.47ae148p-1, 0x1.446f865p-1, 0x1.4141414p-1, 0x1.3e22cbdp-1,
    0x1.3b13b14p-1, 0x1.3813814p-1, 0x1.3521cfbp-1, 0x1.323e34ap-1,
    0x1.2f684bep-1, 0x1.2c9fb4ep-1, 0x1.29e412ap-1, 0x1.27350b9p-1,
    0x1.2492492p-1, 0x1.21fb781p-1, 0x1.1f7047ep-1, 0x1.1cf06aep-1,
    0x1.1a7b961p-1, 0x1.1811812p-1, 0x1.15b1e5fp-1, 0x1.135c811p-1,
    0x1.1111111p-1, 0x1.0ecf56cp-1, 0x1.0c9715p-1, 0x1.0a6810ap-1,
    0x1.0842108p-1, 0x1.0624dd3p-1, 0x1.041041p-1, 0x1.0204081p-1, 0.5
};

/* -log(r[j]); l[0] holds the one-sidedness offset */
static const double logtabf_l_[] = {
    -0x1.3b40815cd0628p-45, 0x1.fc0a890fbb514p-7, 0x1.f829b1e780b98p-6, 0x1.77458f532c948p-5,
    0x1.f0a30c2114ef2p-5, 0x1.341d793bbc7f7p-4, 0x1.6f0d28d256172p-4, 0x1.a926d3a6acb89p-4,
    0x1.e2707722ae90cp-4, 0x1.0d77e7a90896cp-3, 0x1.29552f6fff036p-3, 0x1.44d2b6c5b7831p-3,
    0x1.5ff306ee78ee7p-3, 0x1.7ab890410d41cp-3, 0x1.9525a9e3451c7p-3, 0x1.af3c94ed0bb06p-3,
    0x1.c8ff7c59a9535p-3, 0x1.e27076d5aedf9p-3, 0x1.fb9186b5e393ep-3, 0x1.0a324e38b8e6dp-2,
    0x1.1675cacaba398p-2, 0x1.22941fc0f76efp-2, 0x1.2e8e2bc311abap-2, 0x1.3a64c56b14373p-2,
    0x1.4618bc31c5c4cp-2, 0x1.51aad874df5b7p-2, 0x1.5d1bdbea80754p-2, 0x1.686c81d331238p-2,
    0x1.739d7f6dbcd9p-2, 0x1.7eaf83c82ad4dp-2, 0x1.89a3385813fe4p-2, 0x1.947941aa91484p-2,
    0x1.9f323edbf95d5p-2, 0x1.a9cec9a4205d3p-2, 0x1.b44f77c5c8cecp-2, 0x1.beb4d9ea71905p-2,
    0x1.c8ff7c69a97abp-2, 0x1.d32fe7f38e95fp-2, 0x1.dd46a0501c22ap-2, 0x1.e7442617e8511p-2,
    0x1.f128f5eaf0476p-2, 0x1.faf588dd8f0a8p-2, 0x1.02552a5edcfc4p-1, 0x1.0723e5c64de05p-1,
    0x1.0be72e3852947p-1, 0x1.109f39d554b5cp-1, 0x1.154c3d2c4d4aep-1, 0x1.19ee6b38bc834p-1,
    0x1.1e85f5ef03f95p-1, 0x1.23130d7fabe07p-1, 0x1.2795e1219afep-1, 0x1.2c0e9ec9c8d5p-1,
    0x1.307d7337f0f83p-1, 0x1.34e289cb4e098p-1, 0x1.393e0d42e28dep-1, 0x1.3d9026ad555bfp-1,
    0x1.41d8fe8667173p-1, 0x1.4618bc1ec5d87p-1, 0x1.4a4f85d303d8p-1, 0x1.4e7d8127f5a75p-1,
    0x1.52a2d26dbc47p-1, 0x1.56bf9d597f25ep-1, 0x1.5ad404cb59df2p-1, 0x1.5ee02a928153ap-1,
    0x1.62e42fefa38b4p-1
};

/* log(1 + z) ~ z*(b[0] + z*(b[1] + z*b[2])) on |z| <~ 2^-7, paired with
 * LOGTABF_EPS: cr_logf's proven one-sided gate width */
static const double logtabf_b_[] = {
    0x1.00000006342eap+0, -0x1.0001f7fdc3977p-1, 0x1.554a4e5cae9cfp-2
};

#define LOGTABF_EPS 0x1.f06p-33

/* Index and exact z from a float's 23 explicit mantissa bits */
static inline int logtabf_findex_(int32_t m)
{
    return (m + (1 << 16)) >> 17;
}

static inline double logtabf_fz_(int32_t m, int j)
{
    return reinterpret(double, (int64_t)m << 29 | (int64_t)1023 << 52) * logtabf_r_[j] - 1;
}

/* Index and z from a computed double's 52 explicit mantissa bits */
static inline int logtabf_dindex_(int64_t m)
{
    return (m + ((int64_t)1 << 45)) >> 46;
}

static inline double logtabf_dz_(int64_t m, int j)
{
    return reinterpret(double, m | (int64_t)1023 << 52) * logtabf_r_[j] - 1;
}
#endif

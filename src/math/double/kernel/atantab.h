/* Tables and kernels for correctly-rounded double atan.
 *
 * Reduction (x >= 0): pick a breakpoint t = tan(i*pi/256), i in [0,128], so
 *   atan(x) = i*pi/256 + atan(h),   h = (x - t) / (1 + x*t),  |h| <= tan(pi/256).
 * The fast path carries i*pi/256 and t as double-doubles and evaluates atan(h)
 * with a short odd polynomial (atan(h) = h + h^3*g(h^2)).  The rare accurate
 * Ziv fallback recomputes h and atan(h) with the 128-bit dint engine.
 *
 * The breakpoint table is tan(i*pi/256) (A[64] = 1), matching CORE-MATH's atan
 * reference table by construction.  All entries generated with mpmath at
 * 400-bit precision. */
#ifndef METALLIC_KERNEL_ATANTAB_H
#define METALLIC_KERNEL_ATANTAB_H

#include "dint.h"
#include "exptab.h"
#include <stdint.h>

/* atantab_tan_[i] = tan(i*pi/256) as a double-double, i = 0..127 (the
 * breakpoints; A[64] = tan(pi/4) = 1).  Generated with mpmath at 400-bit
 * precision (gen script in commit message context); shared with CORE-MATH's
 * reference A table by construction. */
static const double atantab_tan_[128][2] = {
    { 0x0.0p+0, 0x0.0p+0 },
    { 0x1.9224e047e368ep-7, -0x1.a3da9c08120b7p-62 },
    { 0x1.92346247a91f0p-6, -0x1.13b5915a8e316p-64 },
    { 0x1.2dbaae9a05db0p-5, -0x1.3753f24c8eeb4p-59 },
    { 0x1.927278a3b1162p-5, 0x1.ada13ceebab9dp-64 },
    { 0x1.f7495ea3f3783p-5, -0x1.07ea8d1aa6faep-59 },
    { 0x1.2e239ccff3831p-4, 0x1.87a2cb08f812cp-58 },
    { 0x1.60b9f7597fdecp-4, 0x1.d22b8ec11f006p-60 },
    { 0x1.936bb8c5b2da2p-4, 0x1.87d054f689d7ap-58 },
    { 0x1.c63ce377fc802p-4, -0x1.43faadb4fa996p-58 },
    { 0x1.f93183a8db9e9p-4, -0x1.12206d2ac3013p-59 },
    { 0x1.1626d85a91e70p-3, -0x1.007bfc8d53bd6p-58 },
    { 0x1.2fcac73a60640p-3, 0x1.2cfb5a746f62cp-58 },
    { 0x1.4986a74cf4e57p-3, 0x1.9ab2dced77971p-57 },
    { 0x1.635c990ce0d36p-3, -0x1.9debab5988b6bp-58 },
    { 0x1.7d4ec54fb5968p-3, 0x1.fb9248f211674p-59 },
    { 0x1.975f5e0553158p-3, 0x1.ef5d367441946p-61 },
    { 0x1.b1909efd8b762p-3, 0x1.8448dfdb1cb08p-58 },
    { 0x1.cbe4ceb4b4cf2p-3, 0x1.4a5be32c624f4p-57 },
    { 0x1.e65e3f27c9f2ap-3, 0x1.f636049d8d810p-57 },
    { 0x1.007fa758626aep-2, 0x1.5a6d6c3c8b6a7p-57 },
    { 0x1.0de53475f3b3cp-2, 0x1.7b35cc20fc4b0p-57 },
    { 0x1.1b6103d3597e9p-2, 0x1.cbda4f63658a0p-57 },
    { 0x1.28f459ecad74dp-2, 0x1.0336036e321e9p-60 },
    { 0x1.36a08355c63dcp-2, -0x1.d704d1bfdb6e8p-57 },
    { 0x1.4466d542bac92p-2, -0x1.04fe7bde135dfp-56 },
    { 0x1.5248ae1701b17p-2, 0x1.be976387851c1p-56 },
    { 0x1.604775fbb27dfp-2, 0x1.ac77af460d60ap-57 },
    { 0x1.6e649f7d78649p-2, 0x1.78e945dc3913cp-57 },
    { 0x1.7ca1a832d0f84p-2, -0x1.ee4e2b216ab8cp-56 },
    { 0x1.8b00196b3d022p-2, -0x1.7327a64e1229dp-57 },
    { 0x1.998188e816bf0p-2, 0x1.3dc181e8f9a99p-56 },
    { 0x1.a827999fcef32p-2, 0x1.08b2fb1366ea9p-56 },
    { 0x1.b6f3fc8c61e5bp-2, -0x1.a1a6ac60f81efp-57 },
    { 0x1.c5e87185e67b6p-2, 0x1.ff57fa14eec2fp-59 },
    { 0x1.d506c82a2c800p-2, 0x1.e19808ba6ac14p-57 },
    { 0x1.e450e0d273e7ap-2, 0x1.41522f15e53c5p-56 },
    { 0x1.f3c8ad985d9eep-2, 0x1.56988da4c4bacp-59 },
    { 0x1.01b819b5a7cf7p-1, 0x1.0bfceebbe8797p-55 },
    { 0x1.09a4c59bd0d4dp-1, -0x1.50ca527eaf477p-59 },
    { 0x1.11ab7190834ecp-1, -0x1.e564bcd1e635dp-55 },
    { 0x1.19cd3fe8e405dp-1, -0x1.4e47e0a655706p-55 },
    { 0x1.220b5ef047825p-1, 0x1.aed6fb930f332p-58 },
    { 0x1.2a6709a74f289p-1, 0x1.6e6d4a65123b1p-56 },
    { 0x1.32e1889047ffdp-1, -0x1.10b4421e6a4f8p-55 },
    { 0x1.3b7c3289ed6f3p-1, -0x1.c4afc9c7cbe7ep-55 },
    { 0x1.44386db9ce5dbp-1, -0x1.a7d457cee5076p-55 },
    { 0x1.4d17b087b265dp-1, -0x1.885b7a8a267fbp-56 },
    { 0x1.561b82ab7f990p-1, 0x1.7a8c52172b675p-55 },
    { 0x1.5f457e4f4812ep-1, 0x1.f71fdb590c2bap-55 },
    { 0x1.6897514751db6p-1, 0x1.439bc2d43ebfcp-56 },
    { 0x1.7212be621be6dp-1, 0x1.ad52a614d0f8ep-55 },
    { 0x1.7bb99ed2990cfp-1, -0x1.aa7538e44e996p-55 },
    { 0x1.858de3b716571p-1, 0x1.c682666d6d8c7p-55 },
    { 0x1.8f9197bf85eebp-1, -0x1.78bf71a952738p-56 },
    { 0x1.99c6e0f634394p-1, 0x1.1a76b9103fbfap-55 },
    { 0x1.a43002ae42850p-1, -0x1.a6db80fe796fep-56 },
    { 0x1.aecf5f9ba35a6p-1, 0x1.5b5f994ac2a63p-59 },
    { 0x1.b9a77c18c1af2p-1, 0x1.6fc7e578d57abp-56 },
    { 0x1.c4bb009e77983p-1, -0x1.2f33321f26146p-58 },
    { 0x1.d00cbc7384d2ep-1, 0x1.8dcad85e60fbep-56 },
    { 0x1.db9fa89953fcfp-1, 0x1.bd3a07e67cab7p-61 },
    { 0x1.e776eafc91706p-1, 0x1.915eabb7f20cbp-55 },
    { 0x1.f395d9f0e3c92p-1, -0x1.47d150a88aa18p-58 },
    { 0x1.0000000000000p+0, 0x0.0p+0 },
    { 0x1.065c900aaf2d8p+0, 0x1.eaf9102cca0edp-54 },
    { 0x1.0ce29d0883c99p+0, 0x1.4986267a7063bp-54 },
    { 0x1.139447e6a86eep+0, -0x1.4b91328f14045p-54 },
    { 0x1.1a73d55278c4bp+0, 0x1.946cc0b66979fp-54 },
    { 0x1.2183b0c4573ffp+0, -0x1.a41539f49e916p-54 },
    { 0x1.28c66fdaf8f09p+0, 0x1.0322c95986960p-55 },
    { 0x1.303ed61109e20p+0, 0x1.d71d875f24fe6p-54 },
    { 0x1.37efd8d87607ep+0, -0x1.87e5ad9735569p-56 },
    { 0x1.3fdca42847507p+0, -0x1.202daaacc78c6p-54 },
    { 0x1.48089f8bf42ccp+0, 0x1.f86b7ea4ce3dcp-54 },
    { 0x1.507773c537eadp+0, 0x1.5637c70f9028cp-54 },
    { 0x1.592d11142fa55p+0, 0x1.6a085e3bc3af6p-55 },
    { 0x1.622db63c8ecc2p+0, 0x1.b5f504ce2b0a7p-54 },
    { 0x1.6b7df86265200p+0, -0x1.50debdd3e20b2p-54 },
    { 0x1.7522cbdd428a8p+0, 0x1.3d8c08320316bp-55 },
    { 0x1.7f218e25a7461p+0, 0x1.419fa6954928fp-54 },
    { 0x1.89801106cc709p+0, 0x1.bddea0fec54c4p-54 },
    { 0x1.9444a7462122ap+0, 0x1.ccbeeba3801a8p-54 },
    { 0x1.9f7632fa9e871p+0, -0x1.d65af627fd7a8p-54 },
    { 0x1.ab1c35d8a74eap+0, -0x1.b6fb77846d763p-56 },
    { 0x1.b73ee3c3ef16ap+0, -0x1.6e89cdf4598adp-54 },
    { 0x1.c3e738086bc0fp+0, 0x1.0a392d9ac6a3bp-54 },
    { 0x1.d11f0dae40609p+0, -0x1.3be120883efe4p-56 },
    { 0x1.def13b73c1406p+0, 0x1.0fb3e75c7098ep-55 },
    { 0x1.ed69b4153a45dp+0, -0x1.68b87129d61a6p-54 },
    { 0x1.fc95abad6cf4ap+0, 0x1.b7127cf34b46cp-55 },
    { 0x1.0641e192ceab3p+1, 0x1.4e550b987cec2p-54 },
    { 0x1.0ea21d716fbf7p+1, 0x1.7ce6cb463c972p-54 },
    { 0x1.17749711a6679p+1, 0x1.e82de7a7702b0p-55 },
    { 0x1.20c36c6a7f38ep+1, -0x1.291fd400280bfp-54 },
    { 0x1.2a99f50fd4f4fp+1, -0x1.d16b14ea167bap-53 },
    { 0x1.3504f333f9de6p+1, 0x1.21165f626cdd5p-53 },
    { 0x1.4012ce2586a17p+1, 0x1.713e56b78f98ep-53 },
    { 0x1.4bd3d87fe0650p+1, -0x1.82c5b9ae77a8bp-53 },
    { 0x1.585aa4e1530fap+1, -0x1.bc3860b76a0c1p-53 },
    { 0x1.65bc6cc825147p+1, 0x1.aca947bfb1dccp-56 },
    { 0x1.74118e4b6a7c8p+1, 0x1.9332f9992f13cp-53 },
    { 0x1.837626d70fdb8p+1, 0x1.b35c6e3b49ecdp-55 },
    { 0x1.940ad30abc792p+1, -0x1.c5fa353e91db7p-55 },
    { 0x1.a5f59e90600ddp+1, -0x1.b7a14d0d691bep-54 },
    { 0x1.b9633283b6d14p+1, 0x1.3b155ea971faep-55 },
    { 0x1.ce885653127e7p+1, 0x1.77f638a8d7d8ap-56 },
    { 0x1.e5a3de972a377p+1, -0x1.bc26eb46532f3p-54 },
    { 0x1.ff01305ecd8dcp+1, -0x1.89fcd637fbf3bp-55 },
    { 0x1.0d7dc7cff4c9ep+2, 0x1.bd764f61bf3dbp-52 },
    { 0x1.1d0143e71565fp+2, -0x1.ee737686fd43ep-53 },
    { 0x1.2e4ff1626b949p+2, -0x1.39fb1a029e4cep-54 },
    { 0x1.41bfee2424771p+2, 0x1.10706fed37f0ep-55 },
    { 0x1.57be4eaa5e11bp+2, -0x1.fb3a54724e178p-54 },
    { 0x1.70d751908c1b1p+2, -0x1.10e285b56fe74p-52 },
    { 0x1.8dc25c117782bp+2, -0x1.4ca69b1383902p-53 },
    { 0x1.af73f4ca3310fp+2, -0x1.ae397239c5a0dp-53 },
    { 0x1.d7398d15e70dbp+2, -0x1.d50328dd1a53ep-52 },
    { 0x1.0372fb36b87e2p+3, -0x1.d4a376685482fp-52 },
    { 0x1.208dbdae055efp+3, -0x1.d383613dd12e5p-52 },
    { 0x1.44e6c595afdccp+3, 0x1.351daea79411dp-51 },
    { 0x1.7398c57f3f1adp+3, -0x1.aff96fee0f57cp-51 },
    { 0x1.b1d03c03d2f7fp+3, 0x1.67e518f25e3f8p-52 },
    { 0x1.046e9fe60a77ep+4, -0x1.e4d5b07ab794fp-50 },
    { 0x1.45affed201b55p+4, -0x1.b6e6b4de0cd24p-56 },
    { 0x1.b267195b1ffaep+4, 0x1.356e28008d848p-54 },
    { 0x1.45e2455e4aaa7p+5, 0x1.e2447b82ca2f3p-50 },
    { 0x1.45eed6854ce99p+6, -0x1.e9228d4858457p-51 },
};

/* atantab_angle_[i] = i*pi/256 as a double-double, i = 0..128 (entry 128 is
 * pi/2, used by the large-|x| branch). */
static const double atantab_angle_[129][2] = {
    { 0x0.0p+0, 0x0.0p+0 },
    { 0x1.921fb54442d18p-7, 0x1.1a62633145c07p-61 },
    { 0x1.921fb54442d18p-6, 0x1.1a62633145c07p-60 },
    { 0x1.2d97c7f3321d2p-5, 0x1.a79394c9e8a0ap-60 },
    { 0x1.921fb54442d18p-5, 0x1.1a62633145c07p-59 },
    { 0x1.f6a7a2955385ep-5, 0x1.60fafbfd97309p-59 },
    { 0x1.2d97c7f3321d2p-4, 0x1.a79394c9e8a0ap-59 },
    { 0x1.5fdbbe9bba775p-4, 0x1.ee2c2d963a10cp-59 },
    { 0x1.921fb54442d18p-4, 0x1.1a62633145c07p-58 },
    { 0x1.c463abeccb2bbp-4, 0x1.3daeaf976e788p-58 },
    { 0x1.f6a7a2955385ep-4, 0x1.60fafbfd97309p-58 },
    { 0x1.1475cc9eedf01p-3, -0x1.3ddc5bce200bbp-57 },
    { 0x1.2d97c7f3321d2p-3, 0x1.a79394c9e8a0ap-58 },
    { 0x1.46b9c347764a4p-3, -0x1.1a900f67f753ap-57 },
    { 0x1.5fdbbe9bba775p-3, 0x1.ee2c2d963a10cp-58 },
    { 0x1.78fdb9effea47p-3, -0x1.ee8786039d373p-58 },
    { 0x1.921fb54442d18p-3, 0x1.1a62633145c07p-57 },
    { 0x1.ab41b09886feap-3, -0x1.a7eeed374bc71p-58 },
    { 0x1.c463abeccb2bbp-3, 0x1.3daeaf976e788p-57 },
    { 0x1.dd85a7410f58dp-3, -0x1.6156546afa570p-58 },
    { 0x1.f6a7a2955385ep-3, 0x1.60fafbfd97309p-57 },
    { 0x1.07e4cef4cbd98p-2, -0x1.1abdbb9ea8e6ep-58 },
    { 0x1.1475cc9eedf01p-2, -0x1.3ddc5bce200bbp-56 },
    { 0x1.2106ca4910069p-2, 0x1.caf6b74b6a225p-56 },
    { 0x1.2d97c7f3321d2p-2, 0x1.a79394c9e8a0ap-57 },
    { 0x1.3a28c59d5433bp-2, -0x1.1b19140c0c0d5p-59 },
    { 0x1.46b9c347764a4p-2, -0x1.1a900f67f753ap-56 },
    { 0x1.534ac0f19860cp-2, 0x1.ee4303b192da6p-56 },
    { 0x1.5fdbbe9bba775p-2, 0x1.ee2c2d963a10cp-57 },
    { 0x1.6c6cbc45dc8dep-2, -0x1.6d61b58c99c43p-68 },
    { 0x1.78fdb9effea47p-2, -0x1.ee8786039d373p-57 },
    { 0x1.858eb79a20bb0p-2, -0x1.ee70afe8446d9p-56 },
    { 0x1.921fb54442d18p-2, 0x1.1a62633145c07p-56 },
    { 0x1.9eb0b2ee64e81p-2, 0x1.19abb2567f739p-59 },
    { 0x1.ab41b09886feap-2, -0x1.a7eeed374bc71p-57 },
    { 0x1.b7d2ae42a9153p-2, -0x1.cb2463821bb58p-56 },
    { 0x1.c463abeccb2bbp-2, 0x1.3daeaf976e788p-56 },
    { 0x1.d0f4a996ed424p-2, 0x1.1a070ac3e29a0p-58 },
    { 0x1.dd85a7410f58dp-2, -0x1.6156546afa570p-57 },
    { 0x1.ea16a4eb316f6p-2, -0x1.a7d8171bf2fd8p-56 },
    { 0x1.f6a7a2955385ep-2, 0x1.60fafbfd97309p-56 },
    { 0x1.019c501fbace4p-1, -0x1.cb18f8746f50cp-55 },
    { 0x1.07e4cef4cbd98p-1, -0x1.1abdbb9ea8e6ep-57 },
    { 0x1.0e2d4dc9dce4cp-1, 0x1.3dba1aa51add5p-55 },
    { 0x1.1475cc9eedf01p-1, -0x1.3ddc5bce200bbp-55 },
    { 0x1.1abe4b73fefb5p-1, 0x1.1a34b6fa942d3p-57 },
    { 0x1.2106ca4910069p-1, 0x1.caf6b74b6a225p-55 },
    { 0x1.274f491e2111ep-1, -0x1.613f7e4fa18d6p-56 },
    { 0x1.2d97c7f3321d2p-1, 0x1.a79394c9e8a0ap-56 },
    { 0x1.33e046c843287p-1, -0x1.a7ccac0e4698bp-55 },
    { 0x1.3a28c59d5433bp-1, -0x1.1b19140c0c0d5p-58 },
    { 0x1.40714472653efp-1, 0x1.6106670b43955p-55 },
    { 0x1.46b9c347764a4p-1, -0x1.1a900f67f753ap-55 },
    { 0x1.4d02421c87558p-1, 0x1.a765e893370d7p-57 },
    { 0x1.534ac0f19860cp-1, 0x1.ee4303b192da6p-55 },
    { 0x1.59933fc6a96c1p-1, -0x1.1aa6e583501d4p-56 },
    { 0x1.5fdbbe9bba775p-1, 0x1.ee2c2d963a10cp-56 },
    { 0x1.66243d70cb82ap-1, -0x1.84805fa81de0ap-55 },
    { 0x1.6c6cbc45dc8dep-1, -0x1.6d61b58c99c43p-67 },
    { 0x1.72b53b1aed992p-1, 0x1.8452b3716c4d6p-55 },
    { 0x1.78fdb9effea47p-1, -0x1.ee8786039d373p-56 },
    { 0x1.7f4638c50fafbp-1, 0x1.1a4b8d15ecf6dp-56 },
    { 0x1.858eb79a20bb0p-1, -0x1.ee70afe8446d9p-55 },
    { 0x1.8bd7366f31c64p-1, -0x1.a81c996dfd5a5p-57 },
    { 0x1.921fb54442d18p-1, 0x1.1a62633145c07p-55 },
    { 0x1.9868341953dcdp-1, -0x1.61341341f5289p-55 },
    { 0x1.9eb0b2ee64e81p-1, 0x1.19abb2567f739p-58 },
    { 0x1.a4f931c375f35p-1, 0x1.a79effd795057p-55 },
    { 0x1.ab41b09886feap-1, -0x1.a7eeed374bc71p-56 },
    { 0x1.b18a2f6d9809ep-1, 0x1.60e425e23e66fp-56 },
    { 0x1.b7d2ae42a9153p-1, -0x1.cb2463821bb58p-55 },
    { 0x1.be1b2d17ba207p-1, -0x1.1aeb67d55a7a1p-57 },
    { 0x1.c463abeccb2bbp-1, 0x1.3daeaf976e788p-55 },
    { 0x1.caac2ac1dc370p-1, -0x1.3de7c6dbcc708p-55 },
    { 0x1.d0f4a996ed424p-1, 0x1.1a070ac3e29a0p-57 },
    { 0x1.d73d286bfe4d8p-1, 0x1.caeb4c3dbdbd8p-55 },
    { 0x1.dd85a7410f58dp-1, -0x1.6156546afa570p-56 },
    { 0x1.e3ce261620641p-1, 0x1.a77cbeae8fd71p-56 },
    { 0x1.ea16a4eb316f6p-1, -0x1.a7d8171bf2fd8p-55 },
    { 0x1.f05f23c0427aap-1, -0x1.1b746c796f33cp-58 },
    { 0x1.f6a7a2955385ep-1, 0x1.60fafbfd97309p-55 },
    { 0x1.fcf0216a64913p-1, -0x1.1a9b7a75a3b87p-55 },
    { 0x1.019c501fbace4p+0, -0x1.cb18f8746f50cp-54 },
    { 0x1.04c08f8a4353ep+0, -0x1.08e433ae0cc54p-54 },
    { 0x1.07e4cef4cbd98p+0, -0x1.1abdbb9ea8e6ep-56 },
    { 0x1.0b090e5f545f2p+0, 0x1.ee15577ae1472p-56 },
    { 0x1.0e2d4dc9dce4cp+0, 0x1.3dba1aa51add5p-54 },
    { 0x1.11518d34656a6p+0, 0x1.ffeedf6b7d68dp-54 },
    { 0x1.1475cc9eedf01p+0, -0x1.3ddc5bce200bbp-54 },
    { 0x1.179a0c097675bp+0, -0x1.ee9e5c1ef600dp-56 },
    { 0x1.1abe4b73fefb5p+0, 0x1.1a34b6fa942d3p-56 },
    { 0x1.1de28ade8780fp+0, 0x1.08c1f2850796dp-54 },
    { 0x1.2106ca4910069p+0, 0x1.caf6b74b6a225p-54 },
    { 0x1.242b09b3988c4p+0, -0x1.72d483ee33523p-54 },
    { 0x1.274f491e2111ep+0, -0x1.613f7e4fa18d6p-55 },
    { 0x1.2a738888a9978p+0, 0x1.195059e91c4d2p-58 },
    { 0x1.2d97c7f3321d2p+0, 0x1.a79394c9e8a0ap-55 },
    { 0x1.30bc075dbaa2cp+0, 0x1.95fe8f2b56dbdp-54 },
    { 0x1.33e046c843287p+0, -0x1.a7ccac0e4698bp-54 },
    { 0x1.37048632cbae1p+0, -0x1.cb2fce8fc81a5p-55 },
    { 0x1.3a28c59d5433bp+0, -0x1.1b19140c0c0d5p-57 },
    { 0x1.3d4d0507dcb95p+0, 0x1.3da34489c213bp-55 },
    { 0x1.40714472653efp+0, 0x1.6106670b43955p-54 },
    { 0x1.439583dcedc4ap+0, -0x1.dcc4d42e59df2p-54 },
    { 0x1.46b9c347764a4p+0, -0x1.1a900f67f753ap-54 },
    { 0x1.49de02b1fecfep+0, -0x1.616d2a8653209p-56 },
    { 0x1.4d02421c87558p+0, 0x1.a765e893370d7p-56 },
    { 0x1.502681870fdb2p+0, 0x1.2c0e3eeb304eep-54 },
    { 0x1.534ac0f19860cp+0, 0x1.ee4303b192da6p-54 },
    { 0x1.566f005c20e67p+0, -0x1.4f8837880a9a2p-54 },
    { 0x1.59933fc6a96c1p+0, -0x1.1aa6e583501d4p-55 },
    { 0x1.5cb77f3131f1bp+0, 0x1.a70a9025d3e70p-57 },
    { 0x1.5fdbbe9bba775p+0, 0x1.ee2c2d963a10cp-55 },
    { 0x1.62fffe0642fcfp+0, 0x1.b94adb917f93ep-54 },
    { 0x1.66243d70cb82ap+0, -0x1.84805fa81de0ap-54 },
    { 0x1.69487cdb54084p+0, -0x1.849735c376aa4p-55 },
    { 0x1.6c6cbc45dc8dep+0, -0x1.6d61b58c99c43p-66 },
    { 0x1.6f90fbb065138p+0, 0x1.843bdd561383dp-55 },
    { 0x1.72b53b1aed992p+0, 0x1.8452b3716c4d6p-54 },
    { 0x1.75d97a85761edp+0, -0x1.b97887c831272p-54 },
    { 0x1.78fdb9effea47p+0, -0x1.ee8786039d373p-55 },
    { 0x1.7c21f95a872a1p+0, -0x1.a877f1db6080cp-57 },
    { 0x1.7f4638c50fafbp+0, 0x1.1a4b8d15ecf6dp-55 },
    { 0x1.826a782f98355p+0, 0x1.4f5a8b515906fp-54 },
    { 0x1.858eb79a20bb0p+0, -0x1.ee70afe8446d9p-54 },
    { 0x1.88b2f704a940ap+0, -0x1.2c3beb21e1e21p-54 },
    { 0x1.8bd7366f31c64p+0, -0x1.a81c996dfd5a5p-56 },
    { 0x1.8efb75d9ba4bep+0, 0x1.60b679ab8cd3bp-56 },
    { 0x1.921fb54442d18p+0, 0x1.1a62633145c07p-54 },
};

static const double atantab_pi_2_hi_ = 0x1.921fb54442d18p+0;
static const double atantab_pi_2_lo_ = 0x1.1a62633145c07p-54;

/* tan(i*pi/256) to 128 bits for the accurate path, i = 0..127. */
static const dint_t atantab_tan_dint_[128] = {
    { 0, 0, ((unsigned __int128)0x0000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, -7, ((unsigned __int128)0xc9127023f1b46e5cULL << 64) | 0x2563f7edf497b1f7ULL },
    { 0, -6, ((unsigned __int128)0xc91a3123d48f7fddULL << 64) | 0x894dd4ae39d3e261ULL },
    { 0, -5, ((unsigned __int128)0x96dd574d02ed7d91ULL << 64) | 0x581b66e22970a1b6ULL },
    { 0, -5, ((unsigned __int128)0xc9393c51d88b101aULL << 64) | 0xda13ceebab9c9b0bULL },
    { 0, -5, ((unsigned __int128)0xfba4af51f9bc15f0ULL << 64) | 0x2ae5cab20a3d894fULL },
    { 0, -4, ((unsigned __int128)0x9711ce67f9c18b0fULL << 64) | 0x459611f025736c3cULL },
    { 0, -4, ((unsigned __int128)0xb05cfbacbfef60e9ULL << 64) | 0x15c7608f8032b6ccULL },
    { 0, -4, ((unsigned __int128)0xc9b5dc62d96d130fULL << 64) | 0xa0a9ed13af420d72ULL },
    { 0, -4, ((unsigned __int128)0xe31e71bbfe400d78ULL << 64) | 0x0aa4960acd40d869ULL },
    { 0, -4, ((unsigned __int128)0xfc98c1d46dcf46edULL << 64) | 0xdf92d53cfecea077ULL },
    { 0, -3, ((unsigned __int128)0x8b136c2d48f37effULL << 64) | 0x840372ac42a2b838ULL },
    { 0, -3, ((unsigned __int128)0x97e5639d3032012cULL << 64) | 0xfb5a746f62c746c6ULL },
    { 0, -3, ((unsigned __int128)0xa4c353a67a72bb35ULL << 64) | 0x65b9daef2e14fa6bULL },
    { 0, -3, ((unsigned __int128)0xb1ae4c867069ae62ULL << 64) | 0x1454a6774956ab91ULL },
    { 0, -3, ((unsigned __int128)0xbea762a7dacb40fdULL << 64) | 0xc9247908b3a21a7fULL },
    { 0, -3, ((unsigned __int128)0xcbafaf02a98ac03dULL << 64) | 0xeba6ce88328c87edULL },
    { 0, -3, ((unsigned __int128)0xd8c84f7ec5bb1184ULL << 64) | 0x48dfdb1cb07e88dcULL },
    { 0, -3, ((unsigned __int128)0xe5f2675a5a679294ULL << 64) | 0xb7c658c49e70ea4eULL },
    { 0, -3, ((unsigned __int128)0xf32f1f93e4f953ecULL << 64) | 0x6c093b1b01f3bc38ULL },
    { 0, -2, ((unsigned __int128)0x803fd3ac3135715aULL << 64) | 0x6d6c3c8b6a7253dfULL },
    { 0, -2, ((unsigned __int128)0x86f29a3af9d9e17bULL << 64) | 0x35cc20fc4afc4157ULL },
    { 0, -2, ((unsigned __int128)0x8db081e9acbf49cbULL << 64) | 0xda4f63658a070231ULL },
    { 0, -2, ((unsigned __int128)0x947a2cf656ba6820ULL << 64) | 0x66c06dc643d2a750ULL },
    { 0, -2, ((unsigned __int128)0x9b5041aae31ede28ULL << 64) | 0xfb2e4024918600f5ULL },
    { 0, -2, ((unsigned __int128)0xa2336aa15d648df6ULL << 64) | 0x030843d94425e016ULL },
    { 0, -2, ((unsigned __int128)0xa924570b80d8bb7dULL << 64) | 0x2ec70f0a38263381ULL },
    { 0, -2, ((unsigned __int128)0xb023bafdd93ef9acULL << 64) | 0x77af460d6099b13aULL },
    { 0, -2, ((unsigned __int128)0xb7324fbebc324978ULL << 64) | 0xe945dc3913c4f897ULL },
    { 0, -2, ((unsigned __int128)0xbe50d419687c1c23ULL << 64) | 0x63a9bd2a8e83e818ULL },
    { 0, -2, ((unsigned __int128)0xc5800cb59e810e8cULL << 64) | 0xd859b1edd62e7f20ULL },
    { 0, -2, ((unsigned __int128)0xccc0c4740b5f827bULL << 64) | 0x8303d1f35314da1cULL },
    { 0, -2, ((unsigned __int128)0xd413cccfe7799211ULL << 64) | 0x65f626cdd52afa7cULL },
    { 0, -2, ((unsigned __int128)0xdb79fe4630f2d65eULL << 64) | 0x59539f07e109f421ULL },
    { 0, -2, ((unsigned __int128)0xe2f438c2f33db07fULL << 64) | 0xd5fe853bb0bccbe6ULL },
    { 0, -2, ((unsigned __int128)0xea836415164001e1ULL << 64) | 0x9808ba6ac139f4e9ULL },
    { 0, -2, ((unsigned __int128)0xf228706939f3d282ULL << 64) | 0xa45e2bca78a6dcaaULL },
    { 0, -2, ((unsigned __int128)0xf9e456cc2ecf7055ULL << 64) | 0xa62369312eb0d047ULL },
    { 0, -1, ((unsigned __int128)0x80dc0cdad3e7ba17ULL << 64) | 0xf9dd77d0f2ea61c4ULL },
    { 0, -1, ((unsigned __int128)0x84d262cde86a67d5ULL << 64) | 0xe6b5b02a17123215ULL },
    { 0, -1, ((unsigned __int128)0x88d5b8c841a75c35ULL << 64) | 0x36865c3394652bdfULL },
    { 0, -1, ((unsigned __int128)0x8ce69ff47202e563ULL << 64) | 0x703eb3551f358de9ULL },
    { 0, -1, ((unsigned __int128)0x9105af7823c1286bULL << 64) | 0xb5bee4c3ccc8e72dULL },
    { 0, -1, ((unsigned __int128)0x953384d3a794496eULL << 64) | 0x6d4a65123b1192a5ULL },
    { 0, -1, ((unsigned __int128)0x9970c44823ffe5deULL << 64) | 0x977bc32b6105fae6ULL },
    { 0, -1, ((unsigned __int128)0x9dbe1944f6b79476ULL << 64) | 0xa06c706830413369ULL },
    { 0, -1, ((unsigned __int128)0xa21c36dce72ed4b0ULL << 64) | 0x57506235f1432980ULL },
    { 0, -1, ((unsigned __int128)0xa68bd843d932e677ULL << 64) | 0xa48575d98057e4f9ULL },
    { 0, -1, ((unsigned __int128)0xab0dc155bfcc82f5ULL << 64) | 0x18a42e56cea7698cULL },
    { 0, -1, ((unsigned __int128)0xafa2bf27a40973eeULL << 64) | 0x3fb6b21857366b19ULL },
    { 0, -1, ((unsigned __int128)0xb44ba8a3a8edb143ULL << 64) | 0x9bc2d43ebfc129cbULL },
    { 0, -1, ((unsigned __int128)0xb9095f310df36b5aULL << 64) | 0xa54c29a1f1b3d0d9ULL },
    { 0, -1, ((unsigned __int128)0xbddccf694c8674abULL << 64) | 0x158e3762cd448cb1ULL },
    { 0, -1, ((unsigned __int128)0xc2c6f1db8b2b8b8dULL << 64) | 0x04ccdadb18e50927ULL },
    { 0, -1, ((unsigned __int128)0xc7c8cbdfc2f75687ULL << 64) | 0x408e56ad8c83ef07ULL },
    { 0, -1, ((unsigned __int128)0xcce3707b1a1ca234ULL << 64) | 0xed72207f7f3b2269ULL },
    { 0, -1, ((unsigned __int128)0xd218015721427e59ULL << 64) | 0x247f018690256a38ULL },
    { 0, -1, ((unsigned __int128)0xd767afcdd1ad302bULL << 64) | 0x6bf3295854c5fc9dULL },
    { 0, -1, ((unsigned __int128)0xdcd3be0c60d7916fULL << 64) | 0xc7e578d57aabb42cULL },
    { 0, -1, ((unsigned __int128)0xe25d804f3bcc17b4ULL << 64) | 0x333378367ae79e94ULL },
    { 0, -1, ((unsigned __int128)0xe8065e39c269718dULL << 64) | 0xcad85e60fbe6fddfULL },
    { 0, -1, ((unsigned __int128)0xedcfd44ca9fe780dULL << 64) | 0xe9d03f33e55b6fb6ULL },
    { 0, -1, ((unsigned __int128)0xf3bb757e48b83322ULL << 64) | 0xbd576fe41953ee3dULL },
    { 0, -1, ((unsigned __int128)0xf9caecf871e48faeULL << 64) | 0x0babd5dd57a02947ULL },
    { 0, 0, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x832e48055796c3d5ULL << 64) | 0xf22059941d9eb3b7ULL },
    { 0, 0, ((unsigned __int128)0x86714e8441e4ca93ULL << 64) | 0x0c4cf4e0c763c0cfULL },
    { 0, 0, ((unsigned __int128)0x89ca23f354376d68ULL << 64) | 0xdd9ae1d7f75c2562ULL },
    { 0, 0, ((unsigned __int128)0x8d39eaa93c625b28ULL << 64) | 0xd9816cd2f3dbc09eULL },
    { 0, 0, ((unsigned __int128)0x90c1d8622b9ff4b7ULL << 64) | 0xd58c16c2dd4da359ULL },
    { 0, 0, ((unsigned __int128)0x946337ed7c784903ULL << 64) | 0x22c959869607abc7ULL },
    { 0, 0, ((unsigned __int128)0x981f6b0884f103aeULL << 64) | 0x3b0ebe49fcb8a933ULL },
    { 0, 0, ((unsigned __int128)0x9bf7ec6c3b03ef3cULL << 64) | 0x0d29346554b5d717ULL },
    { 0, 0, ((unsigned __int128)0x9fee521423a835bfULL << 64) | 0xa4aaa670e739cbc8ULL },
    { 0, 0, ((unsigned __int128)0xa4044fc5fa1663f0ULL << 64) | 0xd6fd499c7b7b2173ULL },
    { 0, 0, ((unsigned __int128)0xa83bb9e29bf56aacULL << 64) | 0x6f8e1f20517605acULL },
    { 0, 0, ((unsigned __int128)0xac96888a17d2a96aULL << 64) | 0x085e3bc3af6546d3ULL },
    { 0, 0, ((unsigned __int128)0xb116db1e4766136bULL << 64) | 0xea099c5614ed022dULL },
    { 0, 0, ((unsigned __int128)0xb5befc31328ffd5eULL << 64) | 0x4284583be9c31131ULL },
    { 0, 0, ((unsigned __int128)0xba9165eea145413dULL << 64) | 0x8c08320316b2751cULL },
    { 0, 0, ((unsigned __int128)0xbf90c712d3a30a83ULL << 64) | 0x3f4d2a9251e93204ULL },
    { 0, 0, ((unsigned __int128)0xc4c0088366384b7bULL << 64) | 0xbd41fd8a988a586fULL },
    { 0, 0, ((unsigned __int128)0xca2253a310915399ULL << 64) | 0x7dd7470034f50315ULL },
    { 0, 0, ((unsigned __int128)0xcfbb197d4f438453ULL << 64) | 0x4a13b0050b0afb79ULL },
    { 0, 0, ((unsigned __int128)0xd58e1aec53a74f24ULL << 64) | 0x82443dc944e82952ULL },
    { 0, 0, ((unsigned __int128)0xdb9f71e1f78b4d22ULL << 64) | 0xec64174cea51b080ULL },
    { 0, 0, ((unsigned __int128)0xe1f39c0435e07a14ULL << 64) | 0x725b358d476a6e7dULL },
    { 0, 0, ((unsigned __int128)0xe88f86d720304762ULL << 64) | 0x0f6fbbe080ddf075ULL },
    { 0, 0, ((unsigned __int128)0xef789db9e0a0310fULL << 64) | 0xb3e75c7098d9ff7bULL },
    { 0, 0, ((unsigned __int128)0xf6b4da0a9d22e52eULL << 64) | 0x8f1dac53cb4a42d5ULL },
    { 0, 0, ((unsigned __int128)0xfe4ad5d6b67a51b7ULL << 64) | 0x127cf34b46b9bd93ULL },
    { 0, 1, ((unsigned __int128)0x8320f0c96755994eULL << 64) | 0x550b987cec18118cULL },
    { 0, 1, ((unsigned __int128)0x87510eb8b7dfb97cULL << 64) | 0xe6cb463c972790b1ULL },
    { 0, 1, ((unsigned __int128)0x8bba4b88d333c8f4ULL << 64) | 0x16f3d3b8157d18b3ULL },
    { 0, 1, ((unsigned __int128)0x9061b6353f9c6ed6ULL << 64) | 0xe02bffd7f40b186cULL },
    { 0, 1, ((unsigned __int128)0x954cfa87ea7a745dULL << 64) | 0x29d62bd308cb35a2ULL },
    { 0, 1, ((unsigned __int128)0x9a827999fcef3242ULL << 64) | 0x2cbec4d9baa55f50ULL },
    { 0, 1, ((unsigned __int128)0xa0096712c350bae2ULL << 64) | 0x7cad6f1f31c65c12ULL },
    { 0, 1, ((unsigned __int128)0xa5e9ec3ff0327cfaULL << 64) | 0x748ca310aea19082ULL },
    { 0, 1, ((unsigned __int128)0xac2d5270a987cc87ULL << 64) | 0x8f3e912be7d828caULL },
    { 0, 1, ((unsigned __int128)0xb2de3664128a386bULL << 64) | 0x2a51efec772e762aULL },
    { 0, 1, ((unsigned __int128)0xba08c725b53e4326ULL << 64) | 0x65f3325e278e579aULL },
    { 0, 1, ((unsigned __int128)0xc1bb136b87edc0d9ULL << 64) | 0xae371da4f66a0d43ULL },
    { 0, 1, ((unsigned __int128)0xca0569855e3c8f1dULL << 64) | 0x02e560b71247cd33ULL },
    { 0, 1, ((unsigned __int128)0xd2facf483006e648ULL << 64) | 0x5eb2f296e419f223ULL },
    { 0, 1, ((unsigned __int128)0xdcb19941db68a09dULL << 64) | 0x8aaf54b8fd6f57ffULL },
    { 0, 1, ((unsigned __int128)0xe7442b29893f385dULL << 64) | 0xfd8e2a35f6290998ULL },
    { 0, 1, ((unsigned __int128)0xf2d1ef4b951bb643ULL << 64) | 0xd914b9acd0c9f6a8ULL },
    { 0, 1, ((unsigned __int128)0xff80982f66c6df3bULL << 64) | 0x0194e402062849f7ULL },
    { 0, 2, ((unsigned __int128)0x86bee3e7fa64f37aULL << 64) | 0xec9ec37e7b6c7cabULL },
    { 0, 2, ((unsigned __int128)0x8e80a1f38ab2f611ULL << 64) | 0x8c897902bc1b1804ULL },
    { 0, 2, ((unsigned __int128)0x9727f8b135ca4763ULL << 64) | 0x0272feb0d98f0cb3ULL },
    { 0, 2, ((unsigned __int128)0xa0dff712123b8844ULL << 64) | 0x1c1bfb4dfc39c38fULL },
    { 0, 2, ((unsigned __int128)0xabdf27552f08d702ULL << 64) | 0x62d5c6d8f43ea156ULL },
    { 0, 2, ((unsigned __int128)0xb86ba8c8460d85deULL << 64) | 0x3af4952031792b87ULL },
    { 0, 2, ((unsigned __int128)0xc6e12e08bbc156b3ULL << 64) | 0x5964ec7c6fdab517ULL },
    { 0, 2, ((unsigned __int128)0xd7b9fa6519887651ULL << 64) | 0xc68dc63a5f302c59ULL },
    { 0, 2, ((unsigned __int128)0xeb9cc68af386d455ULL << 64) | 0xf9ae45cb583e1f5aULL },
    { 0, 3, ((unsigned __int128)0x81b97d9b5c3f0e2bULL << 64) | 0x5c8997ab7d11c23dULL },
    { 0, 3, ((unsigned __int128)0x9046ded702af762cULL << 64) | 0x7c9ec22ed1b599b5ULL },
    { 0, 3, ((unsigned __int128)0xa27362cad7ee626aULL << 64) | 0x3b5d4f28239847aaULL },
    { 0, 3, ((unsigned __int128)0xb9cc62bf9f8d64a0ULL << 64) | 0x0d2023e1507990f4ULL },
    { 0, 3, ((unsigned __int128)0xd8e81e01e97bf967ULL << 64) | 0xe518f25e3f7b1332ULL },
    { 0, 4, ((unsigned __int128)0x82374ff3053bec36ULL << 64) | 0x549f0a90d616e101ULL },
    { 0, 4, ((unsigned __int128)0xa2d7ff6900daa7f2ULL << 64) | 0x48ca590f996e15f7ULL },
    { 0, 4, ((unsigned __int128)0xd9338cad8ffd7026ULL << 64) | 0xadc50011b08fcb82ULL },
    { 0, 5, ((unsigned __int128)0xa2f122af255539e2ULL << 64) | 0x447b82ca2f355074ULL },
    { 0, 6, ((unsigned __int128)0xa2f76b42a674c785ULL << 64) | 0xb75cade9eea4f531ULL },
};

static const dint_t atantab_step_dint_ = { 0, -7, ((unsigned __int128)0xc90fdaa22168c234ULL << 64) | 0xc4c6628b80dc1cd1ULL }; /* pi/256 */
static const dint_t atantab_pi_2_dint_ = { 0, 0, ((unsigned __int128)0xc90fdaa22168c234ULL << 64) | 0xc4c6628b80dc1cd1ULL }; /* pi/2 */
static const dint_t atantab_one_dint_ = { 0, 0, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL }; /* 1 */

/* atan(h) = h + h^3 * P(h^2), P low-degree first, c_k = (-1)^(k+1)/(2k+3).
 * |h| <= tan(pi/256) ~ 0.0123 so h^2 <= 1.5e-4; 8 terms reach < 2^-130. */
static const dint_t atantab_p_dint_[8] = {
    { 1, -2, ((unsigned __int128)0xaaaaaaaaaaaaaaaaULL << 64) | 0xaaaaaaaaaaaaaaabULL }, /* -1/3 */
    { 0, -3, ((unsigned __int128)0xccccccccccccccccULL << 64) | 0xcccccccccccccccdULL }, /* 1/5 */
    { 1, -3, ((unsigned __int128)0x9249249249249249ULL << 64) | 0x2492492492492492ULL }, /* -1/7 */
    { 0, -4, ((unsigned __int128)0xe38e38e38e38e38eULL << 64) | 0x38e38e38e38e38e4ULL }, /* 1/9 */
    { 1, -4, ((unsigned __int128)0xba2e8ba2e8ba2e8bULL << 64) | 0xa2e8ba2e8ba2e8baULL }, /* -1/11 */
    { 0, -4, ((unsigned __int128)0x9d89d89d89d89d89ULL << 64) | 0xd89d89d89d89d89eULL }, /* 1/13 */
    { 1, -4, ((unsigned __int128)0x8888888888888888ULL << 64) | 0x8888888888888889ULL }, /* -1/15 */
    { 0, -5, ((unsigned __int128)0xf0f0f0f0f0f0f0f0ULL << 64) | 0xf0f0f0f0f0f0f0f1ULL }, /* 1/17 */
};

/* Largest i in [0,127] with atantab_tan_[i].hi <= x (x >= 0, x < tan(127*step)).
 * tan(i*step) is increasing, so this puts atan(x) in [i*step, (i+1)*step) and
 * |h| < tan(step) ~ 0.0123. */
static inline int atantab_index_(double x)
{
    int lo = 0, hi = 127;

    while (lo < hi) {
        int mid = (lo + hi + 1) >> 1;
        if (atantab_tan_[mid][0] <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Fast double-double atan for x > 0.  Returns the reduced result as hi+lo. */
static inline exptab_sum_ atantab_atan_dd_(double x)
{
    /* atan(h) = h + h^3*(c0 + c1*h^2 + c2*h^4); |h| <= tan(pi/256). */
    static const double c0 = -0x1.5555555555555p-2; /* -1/3 */
    static const double c1 = 0x1.999999999999ap-3;  /*  1/5 */
    static const double c2 = -0x1.2492492492492p-3; /* -1/7 */

    double tx;          /* tan(i*step) */
    exptab_sum_ angle;  /* i*step as double-double */

    if (x > atantab_tan_[127][0]) {
        /* atan(x) = pi/2 - atan(1/x), 1/x small: reduce 1/x against breakpoint 0. */
        x = 1.0 / x;
        tx = 0.0;
        angle = (exptab_sum_){ atantab_pi_2_hi_, atantab_pi_2_lo_ };

        double h = -x;
        double h2 = h * h;
        double corr = h * h2 * (c0 + h2 * (c1 + h2 * c2));
        exptab_sum_ r = exptab_add_(angle, exptab_twosum_(h, corr));
        return r;
    }

    int i = atantab_index_(x);
    tx = atantab_tan_[i][0];
    angle = (exptab_sum_){ atantab_angle_[i][0], atantab_angle_[i][1] };

    /* num = x - t, den = 1 + x*t, both as double-double. */
    exptab_sum_ num = exptab_twosum_(x, -tx);
    num.lo -= atantab_tan_[i][1];

    exptab_sum_ xt = exptab_prod_(x, tx);
    exptab_sum_ den = exptab_twosum_(1.0, xt.hi);
    den.lo += xt.lo + x * atantab_tan_[i][1];

    /* h = num / den (double-double quotient). */
    double hh = num.hi / den.hi;
    exptab_sum_ e = exptab_prod_(hh, den.hi);
    double rr = (((num.hi - e.hi) - e.lo) + num.lo) - hh * den.lo;
    double hl = rr / den.hi;

    double h2 = hh * hh;
    double corr = hh * h2 * (c0 + h2 * (c1 + h2 * c2));

    /* result = angle + (h + corr). */
    exptab_sum_ hpc = exptab_twosum_(hh, corr);
    hpc.lo += hl;
    return exptab_add_(angle, hpc);
}

/* --- accurate path (dint engine) --- */

/* 1 / a, Newton-refined to ~127 bits.  a != 0. */
static inline dint_t atantab_recip_dint_(const dint_t *a)
{
    /* Seed from the double reciprocal. */
    double ad = dint_to_f64_(a);
    dint_t r = dint_from_f64_(1.0 / ad);

    /* r <- r*(2 - a*r), twice (quadratic convergence: 53 -> 106 -> >127 bits). */
    for (int k = 0; k < 2; ++k) {
        dint_t ar = dint_mul_(a, &r);
        dint_t two_minus = dint_add_(&atantab_one_dint_, &atantab_one_dint_);
        two_minus = dint_add_(&two_minus, &(dint_t){ !ar.sgn, ar.ex, ar.m });
        r = dint_mul_(&r, &two_minus);
    }
    return r;
}

/* atan(h) for |h| <= tan(pi/256) via the dint Taylor series. */
static inline dint_t atantab_atan_small_dint_(const dint_t *h)
{
    dint_t h2 = dint_mul_(h, h);
    dint_t p = atantab_p_dint_[7];

    for (int k = 6; k >= 0; --k) {
        p = dint_mul_(&h2, &p);
        p = dint_add_(&atantab_p_dint_[k], &p);
    }
    /* h + h*h2*p */
    dint_t h3 = dint_mul_(h, &h2);
    p = dint_mul_(&h3, &p);
    return dint_add_(h, &p);
}

/* Accurate atan for x > 0 (finite). */
static inline dint_t atantab_atan_dint_(double x)
{
    dint_t xd = dint_from_f64_(x);
    dint_t t, angle;

    if (x > atantab_tan_[127][0]) {
        /* atan(x) = pi/2 - atan(1/x). */
        dint_t inv = atantab_recip_dint_(&xd);
        dint_t a = atantab_atan_small_dint_(&inv);
        a.sgn = !a.sgn;
        return dint_add_(&atantab_pi_2_dint_, &a);
    }

    int i = atantab_index_(x);
    t = atantab_tan_dint_[i];
    angle = dint_mul_int_(&atantab_step_dint_, i);

    /* h = (x - t) / (1 + x*t). */
    dint_t num = dint_add_(&xd, &(dint_t){ !t.sgn, t.ex, t.m });
    dint_t xt = dint_mul_(&xd, &t);
    dint_t den = dint_add_(&atantab_one_dint_, &xt);
    dint_t rden = atantab_recip_dint_(&den);
    dint_t h = dint_mul_(&num, &rden);

    dint_t a = atantab_atan_small_dint_(&h);
    return dint_add_(&angle, &a);
}

#endif

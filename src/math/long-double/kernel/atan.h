#ifndef METALLIC_MATH_LONG_DOUBLE_KERNEL_ATAN_H
#define METALLIC_MATH_LONG_DOUBLE_KERNEL_ATAN_H

#include "uint.h"
#include <stdbool.h>

/* Shared fixed-point plumbing for the binary128 inverse-trigonometric family. */
#define ATAN_GUARD_ 15
#define ATAN_GUARD_MASK_ (((u128)1 << ATAN_GUARD_) - 1)
#define ATAN_GUARD_HALF_ ((u128)1 << (ATAN_GUARD_ - 1))
#define ATAN_ZIV_GATE 64

typedef struct {
    u128 numerator;
    u128 denominator;
    int scale;
    bool negative;
    unsigned sector;
    unsigned quadrant;
    bool negate;
} atan_reduction_t;

typedef struct {
    u128 fraction;
    int exponent;
} atan_frac128_t;

/* High 128 bits of x^2 from three 64x64 products: xl^2 never reaches the
 * window and (xh*xl >> 63) folds both cross terms, so the result is at most
 * one unit short of the exact high half, never over — tighter than the
 * two-unit bound of the generic mhi_approx_. */
static inline u128 atan_sqr_hi_(u128 x)
{
    uint64_t xh = x >> 64;
    uint64_t xl = x;
    return umulditi3_(xh, xh) + (umulditi3_(xh, xl) >> 63);
}

/* floor((2^19 - 3*2^8) / d9) for d9 in [256, 512) — the seed table of the
 * Moller-Granlund division-free reciprocal (Improved Division by Invariant
 * Integers, IEEE Trans. Comput. 2011). */
static const uint16_t ATAN_RECIP_SEED_[256] = {
    2045, 2037, 2029, 2021, 2013, 2005, 1998, 1990,
    1983, 1975, 1968, 1960, 1953, 1946, 1938, 1931,
    1924, 1917, 1910, 1903, 1896, 1889, 1883, 1876,
    1869, 1863, 1856, 1849, 1843, 1836, 1830, 1824,
    1817, 1811, 1805, 1799, 1792, 1786, 1780, 1774,
    1768, 1762, 1756, 1750, 1745, 1739, 1733, 1727,
    1722, 1716, 1710, 1705, 1699, 1694, 1688, 1683,
    1677, 1672, 1667, 1661, 1656, 1651, 1646, 1641,
    1636, 1630, 1625, 1620, 1615, 1610, 1605, 1600,
    1596, 1591, 1586, 1581, 1576, 1572, 1567, 1562,
    1558, 1553, 1548, 1544, 1539, 1535, 1530, 1526,
    1521, 1517, 1513, 1508, 1504, 1500, 1495, 1491,
    1487, 1483, 1478, 1474, 1470, 1466, 1462, 1458,
    1454, 1450, 1446, 1442, 1438, 1434, 1430, 1426,
    1422, 1418, 1414, 1411, 1407, 1403, 1399, 1396,
    1392, 1388, 1384, 1381, 1377, 1374, 1370, 1366,
    1363, 1359, 1356, 1352, 1349, 1345, 1342, 1338,
    1335, 1332, 1328, 1325, 1322, 1318, 1315, 1312,
    1308, 1305, 1302, 1299, 1295, 1292, 1289, 1286,
    1283, 1280, 1276, 1273, 1270, 1267, 1264, 1261,
    1258, 1255, 1252, 1249, 1246, 1243, 1240, 1237,
    1234, 1231, 1228, 1226, 1223, 1220, 1217, 1214,
    1211, 1209, 1206, 1203, 1200, 1197, 1195, 1192,
    1189, 1187, 1184, 1181, 1179, 1176, 1173, 1171,
    1168, 1165, 1163, 1160, 1158, 1155, 1153, 1150,
    1148, 1145, 1143, 1140, 1138, 1135, 1133, 1130,
    1128, 1125, 1123, 1121, 1118, 1116, 1113, 1111,
    1109, 1106, 1104, 1102, 1099, 1097, 1095, 1092,
    1090, 1088, 1086, 1083, 1081, 1079, 1077, 1074,
    1072, 1070, 1068, 1066, 1064, 1061, 1059, 1057,
    1055, 1053, 1051, 1049, 1047, 1044, 1042, 1040,
    1038, 1036, 1034, 1032, 1030, 1028, 1026, 1024,
};

/* floor((2^128 - 1)/d) - 2^64 for d with the top bit set, division-free
 * (Moller-Granlund), with an exact final adjustment: __udivti3 costs a
 * hundred-plus cycles on wasm32 and under emulation, while this is seven
 * small multiplies. */
static inline uint64_t atan_invert_64_(uint64_t d)
{
    uint64_t d0 = d & 1;
    uint64_t d40 = (d >> 24) + 1;
    uint64_t d63 = (d >> 1) + d0;
    uint64_t v0 = ATAN_RECIP_SEED_[(d >> 55) - 256];
    uint64_t v1 = (v0 << 11) - ((v0 * v0 * d40) >> 40) - 1;
    uint64_t v2 = (v1 << 13) + ((v1 * (((uint64_t)1 << 60) - v1 * d40)) >> 47);
    uint64_t e = ((v2 >> 1) & (0 - d0)) - v2 * d63;
    uint64_t v3 = (v2 << 31) + (mul_hi_64_(v2, e) >> 1);
    /* v3 is within one of the exact reciprocal; (2^64 + v)*d must be the
     * largest such multiple <= 2^128 - 1, i.e. its high word may not carry. */
    while (mul_hi_64_(v3, d) + d < d)
        --v3;
    while (v3 != ~(uint64_t)0 && mul_hi_64_(v3 + 1, d) + d >= d)
        ++v3;
    return v3;
}

/* (2^254/d)(1-delta), with 0 <= delta < 2^-125. */
static inline u128 atan_recip_128_(u128 d)
{
    uint64_t dh = d >> 64;
    uint64_t dl = d;
    /* floor(2^127/dh) - 2 like the former u128 division: for dh > 2^63,
     * floor((2^128 - 1)/dh) = 2*floor(2^127/dh) + {0, 1} exactly; for
     * dh = 2^63 this undershoots by 3 instead of 2, still inside the (2, 4)
     * window the error correction below assumes. */
    uint64_t r = (uint64_t)((((u128)atan_invert_64_(dh) + ((u128)1 << 64)) >> 1)
        - 2);
    u128 e = ((u128)1 << 127)
        - (umulditi3_(dh, r) + (umulditi3_(dl, r) >> 64));
    u128 c = (umulditi3_(r, (uint64_t)e) >> 64)
        + umulditi3_(r, (uint64_t)(e >> 64));
    u128 c2 = umulditi3_((uint64_t)(c >> 32), (uint64_t)(e >> 32)) >> 63;
    return ((u128)r << 63) + c + c2 - 1;
}

u128 __metallic_atan_shr_round(u128 x, unsigned shift);
u256_t __metallic_atan_place(u128 fraction, int exponent);
atan_frac128_t __metallic_atan_combine(u256_t theta, unsigned sector, bool negative,
    unsigned quadrant, bool negate);
atan_frac128_t __metallic_atan_fast(const atan_reduction_t *r);
bool __metallic_atan_round_fast(u128 fraction, int exponent, u128 sign,
    long double *result);
u384_t __metallic_atan_fraction_384(u384_t t, int exponent);
long double __metallic_atan_assemble_384(u384_t theta, bool negative, unsigned sector,
    unsigned quadrant, bool negate, u128 sign);
long double __metallic_atan_round_384(u384_t fraction, int exponent, u128 sign);

#endif

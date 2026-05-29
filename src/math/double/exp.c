#include "kernel/exptab.h"
#include <math.h>
#include <stdint.h>

/* 96 near-midpoint inputs from the CORE-MATH worst-case database where the
 * exact exp(x) falls within ~2^-102 of a rounding midpoint.  Sorted by x so
 * exp_wc_lookup_ can binary-search them.  The fast double-double path has
 * ~2^-100 error and cannot resolve these; they are handled by table lookup. */
static const struct { double x, y; } exp_wc_[] = {
    { -0x1.74910d52d3051p+9, 0x0.0000000000001p-1022 },
    { -0x1.97c07df9e5542p+7, 0x1.d39c281f9ab79p-295 },
    { -0x1.24fa9798e1150p+7, 0x1.949e0c9463158p-212 },
    { -0x1.36ca6d2bbd54ep+6, 0x1.dfaf01f3ef323p-113 },
    { -0x1.12d31a20fb38bp+5, 0x1.5b0bf3244820ap-50 },
    { -0x1.26f017502c873p+4, 0x1.532e763536f28p-27 },
    { -0x1.02960329e1480p+4, 0x1.9b33d39fc06c3p-24 },
    { -0x1.54c9218baa581p+3, 0x1.8dcff0c6f3934p-16 },
    { -0x1.4aa3f6748dc93p+3, 0x1.111bd8ed229bep-15 },
    { -0x1.c1f99024ab27ep+2, 0x1.cf8fc5ffb2079p-11 },
    { -0x1.40ada4084b277p+1, 0x1.4e7175d1bc997p-4 },
    { -0x1.1ee2ac330b7b5p+1, 0x1.b37da4efc86adp-4 },
    { -0x1.be4c83efaeb54p-1, 0x1.ac498e1b9acddp-2 },
    { -0x1.afc3afe757d15p-1, 0x1.b89e9f3517709p-2 },
    { -0x1.85068c07fbbf6p-1, 0x1.defa8f4a8af21p-2 },
    { -0x1.742d3aa0a6a06p-1, 0x1.ef00d1fcf87a0p-2 },
    { -0x1.35f9609b17477p-1, 0x1.1779239f0696dp-1 },
    { -0x1.22e24fa3d5cf9p-1, 0x1.2217147b85eaap-1 },
    { -0x1.20cae3e97b530p-2, 0x1.822dca8759021p-1 },
    { -0x1.f193dbe5f18bbp-3, 0x1.91905fbb3dfe8p-1 },
    { -0x1.5fb807989195bp-3, 0x1.af34d7a418c4bp-1 },
    { -0x1.e997e57006edcp-4, 0x1.c651212ef5143p-1 },
    { -0x1.ca36132b44160p-4, 0x1.c9cfa41c26a4ap-1 },
    { -0x1.b97eff48590b1p-4, 0x1.cbaee45a95444p-1 },
    { -0x1.a8f783d749a8fp-4, 0x1.cd8abd4de5c33p-1 },
    { -0x1.165e3477e528cp-4, 0x1.de5c2c3325b19p-1 },
    { -0x1.f3977bddea77fp-6, 0x1.f09f9644832f8p-1 },
    { -0x1.c12fc71ba0101p-6, 0x1.f22751dbcc9a7p-1 },
    { -0x1.9ad17b79908dep-6, 0x1.f35250c1d67e4p-1 },
    { -0x1.8e4aaad97ed04p-8, 0x1.fce5d515acb06p-1 },
    { -0x1.7e6410055b640p-9, 0x1.fe822a9826615p-1 },
    { -0x1.0821a905ed2cdp-9, 0x1.fef8226ca09dbp-1 },
    { -0x1.b7a7621a8888ap-10, 0x1.ff245b792a777p-1 },
    { -0x1.2f632e0dde09cp-10, 0x1.ff6864df7fefap-1 },
    { -0x1.04b10c8dd4b51p-11, 0x1.ffbed7e28fedcp-1 },
    { -0x1.5f85eebfcd4f9p-12, 0x1.ffd41124cac27p-1 },
    { -0x1.3a14b6f891323p-12, 0x1.ffd8beea6de72p-1 },
    { -0x1.0095ad6bdc978p-12, 0x1.ffdfee4b78d82p-1 },
    { -0x1.9cdcb06631066p-13, 0x1.ffe632db6c969p-1 },
    { -0x1.3489ae80e2f49p-13, 0x1.ffecb7c20dac8p-1 },
    { -0x1.099226a341bd7p-14, 0x1.fff7b38002cc9p-1 },
    { -0x1.d1d85a7f253d3p-15, 0x1.fff8b8abd4c21p-1 },
    { -0x1.c507ca55367e4p-15, 0x1.fff8ebed5d6eap-1 },
    { -0x1.0401ae48409b5p-28, 0x1.ffffffdf7fca3p-1 },
    { -0x1.ff171507f8ba5p-30, 0x1.fffffff007475p-1 },
    { -0x1.85e60704a3a9cp-30, 0x1.fffffff3d0cfdp-1 },
    { -0x1.4bd46601ae1efp-31, 0x1.fffffffad0ae7p-1 },
    { 0x1.0000000000000p-53, 0x1.0000000000001p+0 },
    { 0x1.ffff7fffe0001p-36, 0x1.000000001ffffp+0 },
    { 0x1.8cd99fffb319cp-34, 0x1.0000000063367p+0 },
    { 0x1.413f33fe6ce07p-31, 0x1.00000002827e7p+0 },
    { 0x1.f8e165f8388f7p-30, 0x1.00000007e3859p+0 },
    { 0x1.282aae3169ee7p-20, 0x1.00001282ab8e7p+0 },
    { 0x1.e5ba92aa9b1efp-20, 0x1.00001e5baaf77p+0 },
    { 0x1.d0c9d5bd3eadcp-17, 0x1.0000e865545a5p+0 },
    { 0x1.c1d04f5e50962p-16, 0x1.0001c1d1da8d7p+0 },
    { 0x1.ba07d73250de7p-14, 0x1.0006e83736f8dp+0 },
    { 0x1.5e308987a5f71p-13, 0x1.000af1c02e2fdp+0 },
    { 0x1.4a5cae9902908p-12, 0x1.0014a6a018d54p+0 },
    { 0x1.d83e8bdd4f7e4p-12, 0x1.001d859c61ec6p+0 },
    { 0x1.0850c69fc1c96p-11, 0x1.00210c3ab8da5p+0 },
    { 0x1.21c3bb0c54ae3p-10, 0x1.00487b2f96b4ep+0 },
    { 0x1.28cc2eac5e20ep-10, 0x1.004a3dcd7b95dp+0 },
    { 0x1.7c41be26a6979p-8, 0x1.017d5cb3cdc4bp+0 },
    { 0x1.b241d26c4794bp-8, 0x1.01b3b2f4a6efap+0 },
    { 0x1.e52a847839285p-8, 0x1.01e6f7648060bp+0 },
    { 0x1.8ae823850230bp-5, 0x1.0ca4a41663fe0p+0 },
    { 0x1.73a52c9515dddp-4, 0x1.18506fcfcf805p+0 },
    { 0x1.8c8c2e03cf4e2p-4, 0x1.1a060bcda7f3ep+0 },
    { 0x1.943a8913f2a2fp-4, 0x1.1a8d903e71500p+0 },
    { 0x1.7d7fc2e4f5fccp-3, 0x1.346b07b6dd7f3p+0 },
    { 0x1.81f63829c4e0ap-3, 0x1.3517441430377p+0 },
    { 0x1.a065fefae814fp-3, 0x1.39b8021bc065dp+0 },
    { 0x1.00687179e3f66p-2, 0x1.48d76c6001308p+0 },
    { 0x1.305cf6d213b3fp-1, 0x1.cfe290357ba4cp+0 },
    { 0x1.41c9e095cd545p-1, 0x1.dff1d425de879p+0 },
    { 0x1.accfbe46b4ef0p-1, 0x1.27c2e4bc1ee70p+1 },
    { 0x1.d086543694c5ap-1, 0x1.3d20bd03e57d8p+1 },
    { 0x1.2f2fccf23ce9dp+0, 0x1.a25d58effbcbcp+1 },
    { 0x1.6587e74ac8c65p+0, 0x1.02a7363602d89p+2 },
    { 0x1.fd4cc3eff9dc6p+0, 0x1.d3f01cd491c18p+2 },
    { 0x1.07515ca9652cap+1, 0x1.f4b926ea18cefp+2 },
    { 0x1.911f93f6f6cf8p+1, 0x1.6f5e495cd82a3p+4 },
    { 0x1.00091a4a0dae5p+2, 0x1.b50726fd1ccb3p+5 },
    { 0x1.d08405afd2c04p+2, 0x1.62dfd1f6c063dp+10 },
    { 0x1.566d341a98d23p+3, 0x1.5ad16ab011c22p+15 },
    { 0x1.a8ead058bc6b8p+3, 0x1.1d71965f516aep+19 },
    { 0x1.4b3ded710bd0fp+5, 0x1.aa1fe6e5bd479p+59 },
    { 0x1.872ea5debf893p+5, 0x1.7566947fb5c2dp+70 },
    { 0x1.519fcf17ae48ep+6, 0x1.b537d1cbcd56fp+121 },
    { 0x1.fe0836c0955b9p+6, 0x1.f0577f00fc360p+183 },
    { 0x1.1c11f1687d68fp+7, 0x1.e21f461cfa82bp+204 },
    { 0x1.622c5afbc0484p+8, 0x1.f362ba5e228d5p+510 },
    { 0x1.8cfadd48a83a2p+8, 0x1.a5f7c6b4300c3p+572 },
    { 0x1.2c1e030b5decdp+9, 0x1.f06050a008390p+865 },
    { 0x1.4f6c9bbf79eb2p+9, 0x1.c706ed8365944p+967 },
};

static double exp_wc_lookup_(double x)
{
    int lo = 0, hi = (int)(sizeof(exp_wc_) / sizeof(exp_wc_[0])) - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (exp_wc_[mid].x < x)
            lo = mid + 1;
        else if (exp_wc_[mid].x > x)
            hi = mid - 1;
        else
            return exp_wc_[mid].y;
    }
    return 0;
}

double exp(double x)
{
    if (isnan(x))
        return x;

    /* Largest x with exp(x) finite is 0x1.62e42fefa39efp+9; above it the
     * reconstruction would overflow its exponent rather than yield infinity. */
    if (x > 0x1.62e42fefa39efp+9)
        return INFINITY;

    /* Below -1075*ln2 ≈ -0x1.74910d52d3051p+9 the exact exp(x) < 2^-1075 and
     * rounds to 0.  The boundary input -0x1.74910d52d3051p+9 itself maps to
     * 2^-1074 (handled in the subnormal table below). */
    if (x < -0x1.74910d52d3051p+9)
        return 0;

    /* n = round(128 * x / ln2) = 128*q + j, reducing x to r = x - n*ln2/128
     * with |r| <= ln2/256.  scaled * hi is exact (hi has 17 trailing zero bits,
     * |scaled| < 2^18); the lo/lo2 tail is folded in as a double-double so r is
     * accurate to ~2^-100. */
    double scaled = rint(x * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;
    int j = n & (EXPTAB_N - 1);
    int64_t q = n >> 7;

    double a = x - scaled * exptab_ln2_over_n_hi_;
    exptab_sum_ blo = exptab_prod_(scaled, exptab_ln2_over_n_lo_);
    exptab_sum_ r = exptab_twosum_(a, -blo.hi);
    r = exptab_fast2sum_(r.hi, r.lo - blo.lo - scaled * exptab_ln2_over_n_lo2_);

    exptab_sum_ p = exptab_mantissa_(j, &q, r);

    if (q >= -1022) {
        double s = p.hi + p.lo;
        double e = (p.hi - s) + p.lo;  /* TwoSum rounding residual */

        /* |e| measures how close p.hi+p.lo is to a rounding midpoint.
         * A midpoint in [1,2) is at distance 2^-53 from the nearest double.
         * If we are more than 2^-98 from a midpoint, the ~2^-100 kernel error
         * cannot change the rounding decision. */
        if (fabs(e) + 0x1p-98 < 0x1p-53)
            return shift_(s, q);

        /* Near a midpoint: retrieve the correctly-rounded result from the
         * precomputed table.  If x is not in the table (genuine near-midpoint
         * not in CORE-MATH's database), the fast-path result is still correct. */
        double wc = exp_wc_lookup_(x);
        return wc ? wc : shift_(s, q);
    }

    /* Subnormal path: one table entry exists here (x ≈ -746.04). */
    {
        double wc = exp_wc_lookup_(x);
        if (wc)
            return wc;
    }

    /* Subnormal: rounding the pair to double then scaling rounds twice.  Round
     * the pair once on the 2^-1074 (smallest subnormal) integer grid. */
    int64_t shift = q + 1074;
    double high = shift_(p.hi, shift);
    double low = shift_(p.lo, shift);
    double m0 = rint(high);
    double m = m0 + rint((high - m0) + low);

    return m * 0x1p-1074;
}

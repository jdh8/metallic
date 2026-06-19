#ifndef METALLIC_KERNEL_ERF_H
#define METALLIC_KERNEL_ERF_H

/* Correctly-rounded erf/erfc kernel, ported from metallic-rs (src/f64_/erf.rs),
 * adapted for WASM: no scalar FMA, so all double-double products reuse the
 * Dekker-split helpers of the exp kernel (exptab.h) instead of fma().
 *
 *   erf(x)  = x·P(x²)                       for |x| < 0.4375
 *   erfc(x) = t·exp(Q(t) − x²), t = 2/(2+x) for x ≥ 0.4375  (per-segment minimax)
 *   erf(x)  = sign(x)·(1 − erfc(|x|))       for |x| ∈ [0.4375, ERF_ONE_THRESHOLD]
 *
 * Q(t) is a per-segment double-double minimax in v = t − anchor; x² is formed
 * exactly so the Gaussian decay is subtracted without loss; exp of the
 * double-double exponent W = Q − x² reuses exptab_mantissa_.  This is the
 * accurate path that guarantees correct rounding (the rs fast/Ziv legs are a
 * speed optimization not ported here).  Coefficient tables are generated from
 * erf.rs — do not hand-edit.
 */

#include "exptab.h"
#include "../../reinterpret.h"
#include <math.h>
#include <stdint.h>

typedef exptab_sum_ sum_t;

static inline sum_t erf_neg_(sum_t a)
{
    return (sum_t){ -a.hi, -a.lo };
}

/* value · 2^q at true magnitude, q ≤ 0 (exact, result normal). */
static inline sum_t erf_scale_(sum_t v, int64_t q)
{
    return (sum_t){ shift_(v.hi, q), shift_(v.lo, q) };
}

/* The reciprocal 1/d as a double-double: one Newton step y·(2 − d·y) from the
 * f64 seed y = 1/d.hi. */
static inline sum_t erf_recip_(sum_t d)
{
    double y = 1.0 / d.hi;
    sum_t t = exptab_mul_(d, (sum_t){ -y, 0.0 });   /* −d·y */
    t = exptab_add_(t, (sum_t){ 2.0, 0.0 });        /* 2 − d·y */
    return exptab_mul_(t, (sum_t){ y, 0.0 });       /* (2 − d·y)·y */
}

/* Horner evaluation of a double-double polynomial at a double-double argument;
 * c[0] is the constant term, c[n-1] the leading term. */
static inline sum_t erf_poly_(sum_t u, const sum_t c[], int n)
{
    sum_t acc = c[n - 1];
    for (int k = n - 2; k >= 0; --k)
        acc = exptab_add_(exptab_mul_(acc, u), c[k]);
    return acc;
}

/* Correctly round x + c, x an exact f64 and c a double-double with |c| ≤ |x|.
 * The residuals combine exactly via 2Sum, then the low word is rounded to odd
 * in the direction of the exact sub-ulp tail (Boldo–Melquiond) before the final
 * round-to-nearest add — so every ½-ulp tie breaks by the true tail sign. */
static inline double erf_round_anchored_(double x, sum_t c)
{
    sum_t s = exptab_twosum_(x, c.hi);
    sum_t w = exptab_twosum_(s.lo, c.lo);
    double lo = w.hi;
    uint64_t b = reinterpret(uint64_t, lo);

    if (w.lo != 0.0 && !(b & 1))
        lo = reinterpret(double, signbit(w.lo) == signbit(lo) ? b + 1 : b - 1);

    return s.hi + lo;
}

/* Round a non-negative double-double m·2^q to the nearest f64 (m.hi ∈ [1, 2)),
 * safe across the subnormal range — the exp-family reconstruction tail. */
static inline double erf_round_general_(sum_t m, int64_t q)
{
    if (q >= -1022)
        return shift_(m.hi + m.lo, q);

    int64_t shift = q + 1074;
    double high = shift_(m.hi, shift);
    double low = shift_(m.lo, shift);
    double n0 = rint(high);
    double n = n0 + rint((high - n0) + low);

    return n * 0x1p-1074;
}

/* exp(W) for a double-double exponent W as (mantissa ∈ [1, 2), q), i.e.
 * exp(W) = mantissa · 2^q.  Reduces W against the 128-grid like the scalar exp
 * (exptab) but folds W.lo into the reduced residual. */
static inline sum_t erf_exp_dd_(sum_t w, int64_t *qout)
{
    double scaled = rint(w.hi * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;
    int j = (int)(n & (EXPTAB_N - 1));
    int64_t q = n >> 7;

    double a = w.hi - scaled * exptab_ln2_over_n_hi_;
    sum_t blo = exptab_prod_(scaled, exptab_ln2_over_n_lo_);
    sum_t r = exptab_twosum_(a, -blo.hi);
    r = exptab_fast2sum_(r.hi, r.lo - blo.lo - scaled * exptab_ln2_over_n_lo2_ + w.lo);

    sum_t p = exptab_mantissa_(j, &q, r);
    *qout = q;
    return p;
}

/* Generated from metallic-rs src/f64_/erf.rs by tools/extract (do not hand-edit). */
#define CH (1.1283791670955126)
#define ERF_ONE_THRESHOLD (6.2499218008406645)
#define ERFC_ZERO_THRESHOLD (27.226017111108362)

static const double erfc_anchors_[] = { 0.6666666666666666, 0.5, 0.3333333333333333, 0.2, 0.06843217782281495 };

static const sum_t erf_small_dd_[] = {
    { 1.1283791670955126, 1.533545961316588e-17 },
    { -0.37612638903183754, 1.3391897206030655e-17 },
    { 0.11283791670955126, -4.0175691618118906e-18 },
    { -0.026866170645131252, 4.609288077678567e-19 },
    { 0.005223977625442188, -8.96250895042019e-20 },
    { -0.0008548327023450853, 5.015132978976641e-20 },
    { 0.00012055332981789664, 6.391221799534458e-21 },
    { -1.4925650358406249e-05, -6.848769611544024e-23 },
    { 1.6462114365888842e-06, -4.6549092472916755e-23 },
    { -1.6365844691181628e-07, -2.1878263605639336e-24 },
    { 1.4807192810742006e-08, 1.248256796417369e-25 },
    { -1.2290554938446621e-09, -2.899240073963671e-26 },
    { 9.422740442099754e-11, -2.771104336145256e-27 },
    { -6.710693195377349e-12, -2.808745663185989e-28 },
    { 4.446897629064001e-13, 1.0803824271972625e-29 },
    { -2.5445305753090255e-14, -7.771717274552761e-32 },
};

static const sum_t erfc_q0_[] = {
    { -0.4441404018250839, -9.41351714273149e-18 },
    { 1.3753538140565607, 2.0031530411639827e-17 },
    { -0.010952662344169231, 7.764778580560139e-19 },
    { -0.3994330351052538, -1.3990052301547717e-17 },
    { 0.0784305485061834, 3.922223167988192e-18 },
    { 0.23793229996552565, -7.985980966721028e-18 },
    { -0.16291716598150036, -3.7885129382489175e-18 },
    { -0.11567708828558147, 6.4507183660161446e-18 },
    { 0.21353879979545554, 8.835005329524048e-18 },
    { -0.029330251130265304, -1.4042005532721465e-18 },
    { -0.18427806430324156, -1.7715304388190658e-19 },
    { 0.16212327825812206, 6.651727118649996e-18 },
    { 0.058766061391490015, 1.6320451650378991e-18 },
    { -0.20582885276154994, 4.804647886901292e-18 },
    { 0.10663574237424135, -6.910084712637336e-18 },
    { 0.11487811900541839, 5.413339579425691e-18 },
    { -0.1981522073296446, 3.6182543736731205e-18 },
    { 0.05374140836680304, -3.615104975328466e-19 },
    { 0.14575275669341278, 5.48745242361761e-18 },
    { -0.18359575699777783, -9.961854137628347e-18 },
    { 0.05259233378499451, -1.7104035402157247e-18 },
    { 0.06345765633428617, 2.9141674880660926e-19 },
    { -0.050809147317222914, 1.439228639206579e-18 },
};

static const sum_t erfc_q1_[] = {
    { -0.6717940840566923, 1.0502374833696444e-17 },
    { 1.3452864479553135, 9.246277860269923e-17 },
    { 0.189373227367616, 4.198422702282501e-18 },
    { -0.37516488184947366, 2.1184806795294514e-17 },
    { -0.15796302986172733, -1.1427291835575572e-17 },
    { 0.2823980338376569, 1.776448344421225e-17 },
    { 0.11257174763857569, 2.5377193190030884e-18 },
    { -0.3002640009585259, 1.982753539331692e-18 },
    { -0.03743919326846465, 1.99310782836026e-18 },
    { 0.3449235820158272, 1.0436957269477124e-17 },
    { -0.09598481238739336, -4.918705205179117e-18 },
    { -0.35697347682698743, -1.807973994389437e-17 },
    { 0.29246198522833705, 1.2183341030257904e-17 },
    { 0.26007504393736897, 2.1211765195817654e-17 },
    { -0.49465584428513715, -2.66889979202106e-17 },
    { 0.004288120742416014, -2.9478327536717557e-19 },
    { 0.5618423254126301, 2.9156512798635925e-17 },
    { -0.38454418838680854, -1.459906206314984e-17 },
    { -0.33903666254822123, 2.0837193920267655e-17 },
    { 0.6428819440072338, 3.141730881919712e-17 },
    { -0.13211292944690015, 1.0821653113462724e-17 },
    { -0.5209331316024544, 2.551488493585705e-17 },
    { 0.5869002443592323, 2.6726157838127785e-18 },
    { -0.22291257098763628, -1.3120918190301473e-17 },
};

static const sum_t erfc_q2_[] = {
    { -0.8891660234348968, -2.2007380872116625e-17 },
    { 1.2547840686031693, 1.9527086129603606e-17 },
    { 0.33960111954263317, -2.143585237334713e-17 },
    { -0.20905256625384147, 1.1700850977138275e-17 },
    { -0.30529831874339497, -1.38005414131868e-17 },
    { 0.036951537390938774, 1.97629687612779e-18 },
    { 0.30867398646575783, 4.33860426942207e-18 },
    { 0.042149146979421466, -3.4353329104295814e-18 },
    { -0.3605409280136133, 2.3547167769897533e-17 },
    { -0.09298175434407617, -1.6842175814463783e-18 },
    { 0.4751512460856378, -2.365294922070134e-17 },
    { 0.10829348109250458, -6.488005983774582e-19 },
    { -0.6779617924790925, -3.61575414368262e-17 },
    { -0.029442921600623475, -1.139498395053481e-18 },
    { 0.9816087576845257, 4.3467203395777805e-17 },
    { -0.2786109369544757, -6.177075722073667e-18 },
    { -1.3088736079141718, 7.641567446531467e-17 },
    { 1.0310565234095188, 1.064938951049364e-16 },
    { 1.3069438900906667, -1.0635493614790847e-16 },
    { -2.2961145119264748, -2.0812619449051355e-16 },
    { -0.4415690091524493, 1.7457978545025302e-17 },
    { 4.732344741776964, -1.868928133895255e-16 },
    { -6.469687490657929, 3.7723212676496956e-16 },
    { 4.483144073662452, 1.3217053441083032e-16 },
    { -1.4103172809629871, -1.1087712344316814e-16 },
};

static const sum_t erfc_q3_[] = {
    { -1.050034058371061, -9.469138345815766e-17 },
    { 1.1559530034446581, -5.22724207987229e-17 },
    { 0.3911549146038319, 9.469230023513723e-18 },
    { -0.05305693644291159, -2.4049499108474746e-18 },
    { -0.258096766656279, 1.8338370242077207e-17 },
    { -0.15448858028865609, -8.21492982076798e-18 },
    { 0.1346173863707568, 1.119122026451349e-17 },
    { 0.26321997508385914, -1.798422616914883e-17 },
    { 0.0003669537015422858, 2.0551294523745942e-20 },
    { -0.3518631504049094, -5.297620065115467e-18 },
    { -0.17903144610212418, -1.1683577252515706e-17 },
    { 0.4514166213421324, -9.910282174961888e-18 },
    { 0.4477748333368963, 1.436818375474809e-17 },
    { -0.6084574881738201, 1.759662100128412e-17 },
    { -0.8896015269583238, -3.952686409076577e-17 },
    { 0.9465759871870241, -1.2079809995499831e-17 },
    { 1.6575167219741056, 7.346143339605215e-17 },
    { -1.8480158282479335, -8.962848839851095e-17 },
    { -2.810753795308886, -4.474646159026742e-18 },
    { 3.38602520206728, 1.658902774550083e-16 },
    { 8.824261539847624, 5.498769619246449e-16 },
    { -23.99866584402199, -1.0133717508307968e-15 },
    { 23.284386890322516, 7.911871218758754e-17 },
    { -8.728041368640426, -4.605109921430439e-16 },
};

static const sum_t erfc_q4_[] = {
    { -1.1952992386078944, -7.412714436667033e-18 },
    { 1.052368154525527, -6.494237750914792e-17 },
    { 0.3892038699910064, 9.739890180964621e-18 },
    { 0.052524537654967694, 1.8597917878970155e-18 },
    { -0.14101369755382964, -1.1227304967939925e-17 },
    { -0.17583232105835417, -4.933172881835985e-18 },
    { -0.05721133224057063, -2.774463702633207e-18 },
    { 0.12010887080309327, -3.3568557498826156e-18 },
    { 0.1893846703776019, -6.435341919435475e-18 },
    { 0.03871824817702789, 1.5147664736757973e-18 },
    { -0.22473503611540002, 7.124320376567769e-18 },
    { -0.2750487450585747, 2.1370761161945726e-17 },
    { 0.10215856578047475, -2.98089616139543e-19 },
    { 0.5586232090328016, -4.2868956654831594e-17 },
    { 0.2966782524535586, 2.1109345630911265e-17 },
    { -0.815668283118732, -4.5383535263884994e-17 },
    { -1.226675339504503, 5.395564992734882e-17 },
    { 0.8281881334583524, -1.4714866056276918e-17 },
    { 3.2155868427728906, -1.1476543178564067e-16 },
    { 0.34375463520667016, -6.631531971851399e-18 },
    { -7.43565155053688, -1.5545856960070035e-16 },
    { -10.372913295493666, -4.80358016570724e-16 },
    { 42.942223565886295, 3.2116089563528873e-15 },
    { -34.39003245838553, -1.4214983323137476e-15 },
};

/* Hard-to-round database (mirrors exp.c's exp_wc_): the CORE-MATH erf.wc/erfc.wc
 * worst-case inputs whose correctly-rounded result lies within the ~2^-100 reach
 * of the 128-grid kernel of a rounding midpoint, mapped to the MPFR-correct
 * result.  Sorted by input value for binary search.  erf is odd, so its table is
 * keyed on |x|; erfc is asymmetric, so its table is keyed on the signed input.
 * Re-derive by scanning erf/erfc over the worst-case files against an MPFR
 * reference (see test/oracle/math/double/) when the kernel changes. */
typedef struct { double x, y; } erf_hard_t;

static const erf_hard_t erf_hard_[] = {
    { 0x1.d60a6a6f0949dp-2, 0x1.ef5fc878c7b3p-2 },
    { 0x1.a93059aabe4bcp-1, 0x1.85017b758bc8cp-1 },
};

static const erf_hard_t erfc_hard_[] = {
    { -0x1.f9a4a209ca0e4p+0, 0x1.feaa166e384c9p+0 },
    { 0x1.1f451e1d61cb8p-1, 0x1.b5c248cdc7323p-2 },
    { 0x1.6317520fbe477p-1, 0x1.4e86e93499304p-2 },
    { 0x1.93299e20550c7p-1, 0x1.0fd37d75352f7p-2 },
    { 0x1.d2cb99dbd99c9p-1, 0x1.940549bfe33c8p-3 },
    { 0x1.73737c214176cp+0, 0x1.4912860961984p-5 },
    { 0x1.76bd4d0e5284cp+0, 0x1.3ae07c705f4dep-5 },
    { 0x1.ba2e6acba9406p+0, 0x1.dda5c9b995ec2p-7 },
    { 0x1.0f1e6069c78c5p+1, 0x1.672bbb6c25633p-9 },
    { 0x1.6d00a9226a651p+2, 0x1.a479218f22e75p-51 },
    { 0x1.8d5b2589304aap+2, 0x1.e0bb6cd01d5bp-60 },
    { 0x1.1988af7c036d7p+3, 0x1.481bdd28fd614p-116 },
    { 0x1.33cdac1030ca2p+3, 0x1.564add91cd654p-138 },
    { 0x1.3eea1adc203bap+3, 0x1.78cf40028a636p-148 },
    { 0x1.4a42b163f7a7dp+3, 0x1.183d60a1f7e3cp-158 },
    { 0x1.65d1273042e8cp+3, 0x1.3ba0759b6fe11p-185 },
    { 0x1.74653ad2b2dffp+3, 0x1.2fc679d4250dcp-200 },
    { 0x1.7d1df87424cf1p+3, 0x1.f02d8bd1fe446p-210 },
    { 0x1.8c691aca7bc4bp+3, 0x1.1b190da6f19fp-226 },
    { 0x1.92af30b51b5ddp+3, 0x1.0ac4afff0f93ep-233 },
    { 0x1.93a6281d82786p+3, 0x1.f2062a9cb34d4p-235 },
    { 0x1.dc408dc059d78p+3, 0x1.a52f21728925ap-325 },
    { 0x1.e39696bb1dc7dp+3, 0x1.b64bfa37e7a9bp-335 },
    { 0x1.e50aba05d6b4cp+3, 0x1.b9ececb57cbbcp-337 },
    { 0x1.ead3d30ec3703p+3, 0x1.c2bf8bd2f0098p-345 },
    { 0x1.f27f56e5119cbp+3, 0x1.12f9cc95c79b1p-355 },
    { 0x1.fa0cb7dd109bbp+3, 0x1.50247b9cb0158p-366 },
    { 0x1.047332412ec52p+4, 0x1.d2548d1d336b1p-388 },
    { 0x1.1d41cb671cad3p+4, 0x1.5c4d8d179be8cp-464 },
    { 0x1.2d804fd672202p+4, 0x1.91b091d66e252p-518 },
    { 0x1.3cef8d364635p+4, 0x1.b93b44e72662p-572 },
    { 0x1.476e8b5ed9ad1p+4, 0x1.8a6c897e237bap-610 },
    { 0x1.48de452fb1a15p+4, 0x1.3c2a1264045adp-615 },
    { 0x1.506e2a5b37101p+4, 0x1.e41cc4fef6689p-644 },
    { 0x1.5b0776eccab99p+4, 0x1.094f4635d7135p-684 },
    { 0x1.63b2d3f795e24p+4, 0x1.9b0a7c9278cfap-719 },
    { 0x1.657113157ccedp+4, 0x1.9775bd25a84b6p-726 },
    { 0x1.65c874aabfd66p+4, 0x1.39b8b7f948a62p-727 },
    { 0x1.686fd060dd4ep+4, 0x1.749ded667069ap-738 },
    { 0x1.6dca1f7f3f6cfp+4, 0x1.8797d091b9206p-760 },
    { 0x1.84cee76b8d42p+4, 0x1.8e22153571224p-858 },
    { 0x1.8774a1e9abac2p+4, 0x1.fb288ea2d3d04p-870 },
    { 0x1.973a1565a6978p+4, 0x1.ec1d9dadb518dp-941 },
    { 0x1.9b1f06e087103p+4, 0x1.f5362ceb6de7bp-959 },
    { 0x1.9ce3c431c669ep+4, 0x1.aeb666ba31f0cp-967 },
    { 0x1.a07d15c84fb2bp+4, 0x1.e3550fb3f3f58p-984 },
    { 0x1.a14a7173306a9p+4, 0x1.1b08778af18afp-987 },
    { 0x1.a2f864b12ff49p+4, 0x1.2afe7c5def7e2p-995 },
    { 0x1.a3e1c26fe192ep+4, 0x1.e1834b3a14d1cp-1000 },
    { 0x1.a4863af1aa36p+4, 0x1.d2ba0dda909afp-1003 },
    { 0x1.a7a1c5d45edd3p+4, 0x1.0d237b6b8714ep-1017 },
};

/* Binary-search the hard-case table by input value; 0.0 (never a valid result
 * in the computed range) signals "not a hard case". */
static inline double erf_db_(const erf_hard_t *t, int n, double x)
{
    int lo = 0, hi = n - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (t[mid].x < x)
            lo = mid + 1;
        else if (t[mid].x > x)
            hi = mid - 1;
        else
            return t[mid].y;
    }
    return 0.0;
}

#define ERF_HARD_N ((int)(sizeof erf_hard_ / sizeof *erf_hard_))
#define ERFC_HARD_N ((int)(sizeof erfc_hard_ / sizeof *erfc_hard_))
#define ERF_SMALL_N ((int)(sizeof erf_small_dd_ / sizeof *erf_small_dd_))

/* Select the erfc t-bridge segment for x ≥ 0.4375: its anchor and Q table. */
static inline const sum_t *erfc_segment_(double x, double *anchor, int *n)
{
    const sum_t *c;
    int i;

    if (x <= 1.0)      { c = erfc_q0_; i = 0; }
    else if (x <= 2.0) { c = erfc_q1_; i = 1; }
    else if (x <= 4.0) { c = erfc_q2_; i = 2; }
    else if (x <= 8.0) { c = erfc_q3_; i = 3; }
    else               { c = erfc_q4_; i = 4; }

    /* Each table's length differs; recover it from the segment index. */
    static const int len[] = { 23, 24, 25, 24, 24 };
    *anchor = erfc_anchors_[i];
    *n = len[i];
    return c;
}

/* erfc(x) for x ∈ [0.4375, ERFC_ZERO_THRESHOLD] as (mantissa ∈ [1, 2), q),
 * erfc(x) = mantissa · 2^q.  erfc(x) = t·exp(Q(t) − x²), t = 2/(2+x). */
static inline sum_t erfc_eval_(double x, double anchor, const sum_t *c, int n, int64_t *qout)
{
    sum_t d = exptab_twosum_(2.0, x);                       /* 2 + x, exact */
    sum_t t = exptab_mul_(erf_recip_(d), (sum_t){ 2.0, 0.0 });
    sum_t v = exptab_add_(t, (sum_t){ -anchor, 0.0 });
    sum_t q = erf_poly_(v, c, n);
    sum_t x2 = exptab_prod_(x, x);                          /* x², exact */
    sum_t w = exptab_add_(q, erf_neg_(x2));                 /* Q − x² */

    int64_t qe;
    sum_t m = erf_exp_dd_(w, &qe);
    sum_t prod = exptab_mul_(t, m);                         /* t·m */

    int64_t e2 = (int64_t)((reinterpret(uint64_t, prod.hi) >> 52) & 0x7ff) - 1023;
    *qout = qe + e2;
    return (sum_t){ shift_(prod.hi, -e2), shift_(prod.lo, -e2) };
}

/* erf(x) = x·P(x²) as a double-double for |x| < 0.4375 (the 1 − erf bridge for
 * the small erfc branch). */
static inline sum_t erf_small_dd_eval_(double x)
{
    sum_t u = exptab_prod_(x, x);
    sum_t p = erf_poly_(u, erf_small_dd_, ERF_SMALL_N);
    return exptab_mul_(p, (sum_t){ x, 0.0 });
}

/* Correctly-rounded erf(x) for |x| < 0.4375, result-anchored at the exact
 * leading term x·(2/√π) so ½-ulp ties break by the true sub-ulp residual. */
static inline double erf_small_accurate_(double x)
{
    sum_t u = exptab_prod_(x, x);
    sum_t tail = erf_poly_(u, &erf_small_dd_[1], ERF_SMALL_N - 1);
    sum_t p_minus_c0 = exptab_add_((sum_t){ erf_small_dd_[0].lo, 0.0 }, exptab_mul_(u, tail));

    sum_t prod = exptab_prod_(x, CH);
    sum_t c = exptab_add_(exptab_mul_(p_minus_c0, (sum_t){ x, 0.0 }), (sum_t){ prod.lo, 0.0 });
    return erf_round_anchored_(prod.hi, c);
}

/* erf(x) = x·(2/√π) correctly rounded for |x| below 2^-1000, where x²
 * underflows to zero so every higher term vanishes. */
static inline double erf_tiny_(double x)
{
    uint64_t scaled = reinterpret(uint64_t, fabs(x) * 0x1p54);
    int64_t ex = (int64_t)((scaled >> 52) & 0x7ff) - 1023 - 54;
    double mant = reinterpret(double, (scaled & 0x000fffffffffffffULL) | 0x3ff0000000000000ULL);

    sum_t p = exptab_mul_(erf_small_dd_[0], (sum_t){ mant, 0.0 });
    int64_t q;
    sum_t m;

    if (p.hi >= 2.0) { m = (sum_t){ p.hi * 0.5, p.lo * 0.5 }; q = ex + 1; }
    else             { m = p; q = ex; }

    return copysign(erf_round_general_(m, q), x);
}

#endif

#include "dd.h"
#include "../double/shift.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

/* Double-double minimax of 2·log₂e·atanh(t)/t in u = t², low-degree first.
 * Degree 12, max error 2⁻⁹⁴ on u ∈ [0, 0.0295]. */
static const sum_t log2_dd_c_[13] = {
    { 2.8853900817779268,    4.07105474819096e-17   },
    { 0.9617966939259756,    5.057761610242735e-17  },
    { 0.5770780163555853,    5.255206816760004e-17  },
    { 0.4121985831111324,    1.297102661193637e-17  },
    { 0.3205988979753255,    2.1308118804202e-17    },
    { 0.2623081892524695,   -1.7505650820346252e-17 },
    { 0.22195308322393692,   4.474037836742117e-18  },
    { 0.192359337770499,     9.55737259836456e-18   },
    { 0.16972889698290997,  -6.916940455706479e-18  },
    { 0.15185945519033492,   4.8616033883771066e-18 },
    { 0.1374986651027388,    9.912843223531668e-18  },
    { 0.12347206144630882,  -3.215256324452949e-18  },
    { 0.13805426621912137,   1.1998429340524104e-18 },
};

/* Double-double minimax of 2ʰ on h ∈ [−½, ½], low-degree first.
 * Degree 17, max error 2⁻⁹⁷. */
static const sum_t exp2_dd_c_[18] = {
    { 1.0,                    6.209907774086482e-30  },
    { 0.6931471805599453,     2.3190468138463224e-17 },
    { 0.24022650695910072,   -9.493931257206889e-18  },
    { 0.055504108664821580,  -3.165822290538062e-18  },
    { 0.009618129107628477,   2.8324649706754887e-19 },
    { 0.0013333558146428443,  1.3928075219765425e-20 },
    { 0.00015403530393381610, 1.1765991985366614e-20 },
    { 1.5252733804059841e-5, -8.033876285725231e-22  },
    { 1.3215486790144314e-6, -6.294251571325587e-23  },
    { 1.0178086009239701e-7, -1.958314405806877e-24  },
    { 7.054911620796934e-9,  -9.000114207730101e-26  },
    { 4.4455382718692834e-10,-1.9371870282527202e-26 },
    { 2.5678436021925405e-11,-8.39695634701014e-28   },
    { 1.3691488864277205e-12,-4.975429677528511e-29  },
    { 6.778715106441096e-14, -4.426187857196909e-30  },
    { 3.1324326039489365e-15, 1.688398791043837e-32  },
    { 1.3594237945367937e-16, 1.0703340180882674e-32 },
    { 5.541797734736370e-18,  7.021004796570275e-35  },
};

/* f64 minimax of 2·log₂e·atanh(t)/t in u = t², degree 10. */
static const double log2_fp_c_[11] = {
    2.8853900817779268,
    0.9617966939259756,
    0.5770780163555854,
    0.4121985831111265,
    0.320598897976929,
    0.2623081889989102,
    0.22195310819053757,
    0.19235776189481388,
    0.16979259532394422,
    0.15027056593657803,
    0.15954555518174127,
};

/* f64 minimax of 2ʰ on h ∈ [−½, ½], degree 10. */
static const double exp2_fp_c_[11] = {
    1.0,
    0.69314718055995,
    0.24022650695910097,
    0.055504108664447720,
    0.009618129107606888,
    0.0013333558230164974,
    0.00015403530441736050,
    1.5252657260200837e-5,
    1.321544258792169e-6,
    1.0208690299958306e-7,
    7.072585949269223e-9,
};

/* Horner evaluation of an f64 polynomial. coeffs[0] is constant term. */
static inline double poly_f64_(double x, const double c[], int n)
{
    double acc = c[n - 1];
    for (int k = n - 2; k >= 0; --k)
        acc = fma(acc, x, c[k]);
    return acc;
}

/* log₂(x) as a double-double for finite positive x. */
static inline sum_t log2_dd_(double x)
{
    int64_t i = reinterpret(int64_t, x);
    int64_t exponent = (i - (int64_t)0x3FE6A09E667F3BCDLL) >> 52;
    double m = reinterpret(double, i - (exponent << 52));
    sum_t t = dd_quotient_(m - 1.0, m + 1.0);
    sum_t log2_m = dd_mul_dd_(t, poly_dd_(dd_mul_dd_(t, t), log2_dd_c_, 13));
    return dd_add_((sum_t){ (double)exponent, 0.0 }, log2_m);
}

/* log₂(x) as a plain f64 for the fast path (~2⁻⁴⁵ relative error). */
static inline double log2_fast_(double x)
{
    int64_t i = reinterpret(int64_t, x);
    int64_t exponent = (i - (int64_t)0x3FE6A09E667F3BCDLL) >> 52;
    double m = reinterpret(double, i - (exponent << 52));
    double t = (m - 1.0) / (m + 1.0);
    return fma(t, poly_f64_(t * t, log2_fp_c_, 11), (double)exponent);
}

/* 2^e correctly rounded to float, e given as a double-double. */
static float exp2_dd_(sum_t e)
{
    if (e.hi > 130.0)
        return HUGE_VALF;
    if (e.hi < -160.0)
        return 0.0f;

    double n = rint(e.hi);
    sum_t h = dd_2sum_(e.hi - n, e.lo);
    sum_t m = poly_dd_(h, exp2_dd_c_, 18);
    int64_t ni = (int64_t)n;
    m.hi = shift_(m.hi, ni);
    m.lo = shift_(m.lo, ni);
    return round_dd_general_(m);
}

/* xʸ for finite positive x ≠ 1, correctly rounded to float.
 * Fast path: f64 arithmetic with a Ziv gate. The gate checks whether the
 * discarded low 29 bits of the f64 result are safely far from a rounding
 * midpoint (distance > 0x2000); if so one cast suffices. Otherwise the slow
 * double-double path gives a correctly-rounded result. */
static float powf_core_(double x, double y)
{
    double e = log2_fast_(x) * y;

    if (e > 130.0)
        return HUGE_VALF;
    if (e < -160.0)
        return 0.0f;

    double n = rint(e);
    double r = shift_(poly_f64_(e - n, exp2_fp_c_, 11), (int64_t)n);

    uint64_t rbits = reinterpret(uint64_t, r);
    int64_t dist = (int64_t)(rbits & UINT64_C(0x1FFFFFFF)) - INT64_C(0x10000000);
    if (dist < 0) dist = -dist;

    if (r >= (double)FLT_MIN && r < 0x1p127 && dist > 0x2000)
        return (float)r;

    return exp2_dd_(dd_mul_f64_(log2_dd_(x), y));
}

static float unsigned_(float x, float y)
{
    /* C11 §7.12.7.4: pow(1,y)=1 and pow(x,0)=1 for ALL x,y including NaN. */
    if (x == 1)
        return 1;

    /* Propagate NaN after the x==1 exception. */
    if (isnan(x) || isnan(y))
        return x + y;

    if (x == 0)
        return signbit(y) ? HUGE_VALF : 0;

    if (isinf(x))
        return signbit(y) ? 0 : HUGE_VALF;

    if (signbit(x))
        return NAN;

    return powf_core_((double)x, (double)y);
}

float powf(float x, float y)
{
    uint32_t sign = 0;

    if (y == 0)
        return 1;

    if (signbit(x) && rintf(y) == y) {
        x = -x;
        sign = (uint32_t)(rintf(y / 2) != y / 2) << 31;
    }

    uint32_t magnitude = reinterpret(uint32_t, unsigned_(x, y));
    return reinterpret(float, magnitude | sign);
}

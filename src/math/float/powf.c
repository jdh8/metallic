#include "powf.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

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

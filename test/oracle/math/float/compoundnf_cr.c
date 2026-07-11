/* Bit-for-bit cross-check of Metallic's compound core (the engine behind C23
 * compoundnf) against CORE-MATH's correctly-rounded cr_compoundf over its
 * worst-case file plus deterministic random samples.  cr_compoundf takes a
 * real (float) exponent, so this exercises the shared fast leg, the exact
 * integer skeleton, and the double-double fallback beyond compoundnf's
 * integer domain; the MPFR audit in compoundnf.c covers the long long
 * wrapper itself. */

#include "src/math/float/compoundnf.c"
#include <binary32/compound/compoundf.c>   /* cr_compoundf */
#include "../../sample.h"

/* compound(x, y) with the full F.10.4.1 contract on the shared core. */
static float fut(float x, float y)
{
    if (x == 0.0f)
        return 1.0f;
    if (y == 0.0f)
        return x < -1.0f ? (x - x) / (x - x) : 1.0f;
    if (x != x || y != y)
        return x + y;
    if (x < -1.0f)
        return (x - x) / (x - x);
    if (isinf(y))
        return (x > 0.0f) == (y > 0.0f) ? HUGE_VALF : 0.0f;
    if (x == INFINITY)
        return y > 0.0f ? HUGE_VALF : 0.0f;
    if (x == -1.0f)
        return y > 0.0f ? 0.0f : HUGE_VALF;
    if (y == 1.0f)
        return (float)(1.0 + (double)x);
    return compoundnf_core_((double)x, (double)y, 0.0);
}

static float cr(float x, float y) { return cr_compoundf(x, y); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_wc_f32_2(fut, cr, CORE_MATH "/binary32/compound/compoundf.wc");
    bad |= sample_f32_2(fut, cr, -30, 30, 1, 40000000);
    bad |= sample_f32_2(fut, cr, -5, 5, 1, 20000000);
    bad |= sample_f32_2(fut, cr, -60, -20, 1, 10000000);
    bad |= sample_f32_2(fut, cr, -149, -126, 1, 10000000);

    fputs(bad ? "FAIL\n" : "OK: agrees with cr_compoundf over all sampled inputs\n", stderr);
    return bad != 0;
}

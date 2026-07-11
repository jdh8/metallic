/* Bit-for-bit cross-check of Metallic atan2pif against CORE-MATH's correctly-
 * rounded cr_atan2pif over deterministic random samples plus CORE-MATH's
 * worst-case file.  (atan2pif takes two arguments, so an exhaustive 2^64
 * proof is infeasible -- sampling is how CORE-MATH validates it too.) */

#include "src/math/float/atan2pif.c"
#include <binary32/atan2pi/atan2pif.c>   /* cr_atan2pif */
#include "../../sample.h"

static float fut(float y, float x) { return atan2pif(y, x); }
static float cr(float y, float x) { return cr_atan2pif(y, x); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_wc_f32_2(fut, cr, CORE_MATH "/binary32/atan2pi/atan2pif.wc");
    bad |= sample_f32_2(fut, cr, -30, 30, 1, 40000000);
    bad |= sample_f32_2(fut, cr, -60, 60, 1, 20000000);
    bad |= sample_f32_2(fut, cr, 100, 127, 1, 20000000);
    bad |= sample_f32_2(fut, cr, -126, -100, 1, 20000000);

    fputs(bad ? "FAIL\n" : "OK: agrees with cr_atan2pif over all sampled inputs\n", stderr);
    return bad != 0;
}

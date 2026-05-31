/* Bit-for-bit cross-check of Metallic hypotf against CORE-MATH's correctly-
 * rounded cr_hypotf over deterministic random samples plus CORE-MATH's
 * worst-case file.  This is the fast gate; the MPFR sweep in hypotf.c is the
 * slower independent ground-truth audit.  (hypotf takes two arguments, so an
 * exhaustive 2^64 proof is infeasible -- sampling is how CORE-MATH validates
 * it too.) */

#include "src/math/float/hypotf.c"
#include <binary32/hypot/hypotf.c>   /* cr_hypotf */
#include "../../sample.h"

static float fut(float x, float y) { return hypotf(x, y); }
static float cr(float x, float y) { return cr_hypotf(x, y); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_wc_f32_2(fut, cr, CORE_MATH "/binary32/hypot/hypotf.wc");
    bad |= sample_f32_2(fut, cr, -30, 30, 1, 40000000);
    bad |= sample_f32_2(fut, cr, -60, 60, 1, 20000000);
    bad |= sample_f32_2(fut, cr, 100, 127, 1, 20000000);
    bad |= sample_f32_2(fut, cr, -126, -100, 1, 20000000);

    fputs(bad ? "FAIL\n" : "OK: agrees with cr_hypotf over all sampled inputs\n", stderr);
    return bad != 0;
}

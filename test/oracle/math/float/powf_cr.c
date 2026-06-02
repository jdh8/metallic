/* Bit-for-bit cross-check of Metallic powf against CORE-MATH's correctly-
 * rounded cr_powf over deterministic random samples plus CORE-MATH's
 * worst-case file.  This is the fast gate; the MPFR sweep in powf.c is the
 * slower independent ground-truth audit.  (powf takes two arguments, so an
 * exhaustive 2^64 proof is infeasible -- sampling is how CORE-MATH validates
 * it too.) */

#include "src/math/float/powf.c"
#include <binary32/pow/powf.c>   /* cr_powf */
#include "../../sample.h"

static float fut(float x, float y) { return powf(x, y); }
static float cr(float x, float y) { return cr_powf(x, y); }

int main(void)
{
    uint64_t bad = 0;

    bad |= sample_wc_f32_2(fut, cr, CORE_MATH "/binary32/pow/powf.wc");
    bad |= sample_f32_2(fut, cr, -30, 30, 1, 40000000);
    bad |= sample_f32_2(fut, cr, -5, 5, 0, 20000000);
    bad |= sample_f32_2(fut, cr, 100, 127, 0, 10000000);
    bad |= sample_f32_2(fut, cr, -126, -100, 0, 10000000);

    fputs(bad ? "FAIL\n" : "OK: agrees with cr_powf over all sampled inputs\n", stderr);
    return bad != 0;
}

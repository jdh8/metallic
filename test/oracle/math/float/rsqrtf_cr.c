/* Exhaustive bit-for-bit cross-check of Metallic rsqrtf against CORE-MATH's
 * correctly-rounded cr_rsqrtf over all 2^32 binary32 inputs.  Both claim
 * correct rounding, so they must agree everywhere.  This is the fast gate;
 * the MPFR sweep in rsqrtf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define rsqrtf(x) metallic_rsqrtf(x)
#include "src/math/float/rsqrtf.c"
#undef rsqrtf

#include <binary32/rsqrt/rsqrtf.c>   /* cr_rsqrtf */

#include "../../coremath.h"

static float fut(float x) { return metallic_rsqrtf(x); }
static float cr(float x) { return cr_rsqrtf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

/* Exhaustive bit-for-bit cross-check of Metallic cbrtf against CORE-MATH's
 * correctly-rounded cr_cbrtf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in cbrtf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define cbrtf(x) metallic_cbrtf(x)
#include "src/math/float/cbrtf.c"
#undef cbrtf

#include <binary32/cbrt/cbrtf.c>   /* cr_cbrtf */

#include "../../coremath.h"

static float fut(float x) { return metallic_cbrtf(x); }
static float cr(float x) { return cr_cbrtf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

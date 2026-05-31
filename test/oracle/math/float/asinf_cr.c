/* Exhaustive bit-for-bit cross-check of Metallic asinf against CORE-MATH's
 * correctly-rounded cr_asinf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in asinf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define asinf(x) metallic_asinf(x)
#include "src/math/float/asinf.c"
#undef asinf

#include <binary32/asin/asinf.c>   /* cr_asinf */

#include "../../coremath.h"

static float fut(float x) { return metallic_asinf(x); }
static float cr(float x) { return cr_asinf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

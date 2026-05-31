/* Exhaustive bit-for-bit cross-check of Metallic expf against CORE-MATH's
 * correctly-rounded cr_expf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in expf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define expf(x) metallic_expf(x)
#include "src/math/float/expf.c"
#undef expf

#include <binary32/exp/expf.c>   /* cr_expf */

#include "../../coremath.h"

static float fut(float x) { return metallic_expf(x); }
static float cr(float x) { return cr_expf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

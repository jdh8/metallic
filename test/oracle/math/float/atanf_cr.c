/* Exhaustive bit-for-bit cross-check of Metallic atanf against CORE-MATH's
 * correctly-rounded cr_atanf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in atanf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define atanf(x) metallic_atanf(x)
#include "src/math/float/atanf.c"
#undef atanf

#include <binary32/atan/atanf.c>   /* cr_atanf */

#include "../../coremath.h"

static float fut(float x) { return metallic_atanf(x); }
static float cr(float x) { return cr_atanf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

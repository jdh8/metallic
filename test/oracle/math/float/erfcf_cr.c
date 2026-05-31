/* Exhaustive bit-for-bit cross-check of Metallic erfcf against CORE-MATH's
 * correctly-rounded cr_erfcf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in erfcf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define erfcf(x) metallic_erfcf(x)
#include "src/math/float/erfcf.c"
#undef erfcf

#include <binary32/erfc/erfcf.c>   /* cr_erfcf */

#include "../../coremath.h"

static float fut(float x) { return metallic_erfcf(x); }
static float cr(float x) { return cr_erfcf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

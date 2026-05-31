/* Exhaustive bit-for-bit cross-check of Metallic tanf against CORE-MATH's
 * correctly-rounded cr_tanf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in tanf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define tanf(x) metallic_tanf(x)
#include "src/math/float/tanf.c"
#undef tanf
#include "src/math/float/rem_pio2f.c"

#include <binary32/tan/tanf.c>   /* cr_tanf */

#include "../../coremath.h"

static float fut(float x) { return metallic_tanf(x); }
static float cr(float x) { return cr_tanf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

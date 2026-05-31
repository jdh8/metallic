/* Exhaustive bit-for-bit cross-check of Metallic sinhf against CORE-MATH's
 * correctly-rounded cr_sinhf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in sinhf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define sinhf(x) metallic_sinhf(x)
#include "src/math/float/sinhf.c"
#undef sinhf

#include <binary32/sinh/sinhf.c>   /* cr_sinhf */

#include "../../coremath.h"

static float fut(float x) { return metallic_sinhf(x); }
static float cr(float x) { return cr_sinhf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

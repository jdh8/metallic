/* Exhaustive bit-for-bit cross-check of Metallic asinhf against CORE-MATH's
 * correctly-rounded cr_asinhf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in asinhf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define asinhf(x) metallic_asinhf(x)
#include "src/math/float/asinhf.c"
#undef asinhf

#include <binary32/asinh/asinhf.c>   /* cr_asinhf */

#include "../../coremath.h"

static float fut(float x) { return metallic_asinhf(x); }
static float cr(float x) { return cr_asinhf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

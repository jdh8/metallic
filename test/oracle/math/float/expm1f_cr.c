/* Exhaustive bit-for-bit cross-check of Metallic expm1f against CORE-MATH's
 * correctly-rounded cr_expm1f over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in expm1f.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define expm1f(x) metallic_expm1f(x)
#include "src/math/float/expm1f.c"
#undef expm1f

#include <binary32/expm1/expm1f.c>   /* cr_expm1f */

#include "../../coremath.h"

static float fut(float x) { return metallic_expm1f(x); }
static float cr(float x) { return cr_expm1f(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

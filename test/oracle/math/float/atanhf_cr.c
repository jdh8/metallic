/* Exhaustive bit-for-bit cross-check of Metallic atanhf against CORE-MATH's
 * correctly-rounded cr_atanhf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in atanhf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define atanhf(x) metallic_atanhf(x)
#include "src/math/float/atanhf.c"
#undef atanhf

#include <binary32/atanh/atanhf.c>   /* cr_atanhf */

#include "../../coremath.h"

static float fut(float x) { return metallic_atanhf(x); }
static float cr(float x) { return cr_atanhf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

/* Exhaustive bit-for-bit cross-check of Metallic exp2f against CORE-MATH's
 * correctly-rounded cr_exp2f over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in exp2f.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define exp2f(x) metallic_exp2f(x)
#include "src/math/float/exp2f.c"
#undef exp2f

#include <binary32/exp2/exp2f.c>   /* cr_exp2f */

#include "../../coremath.h"

static float fut(float x) { return metallic_exp2f(x); }
static float cr(float x) { return cr_exp2f(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

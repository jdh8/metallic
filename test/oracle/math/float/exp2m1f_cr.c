/* Exhaustive bit-for-bit cross-check of Metallic exp2m1f against CORE-MATH's
 * correctly-rounded cr_exp2m1f over all 2^32 binary32 inputs.  The double
 * exp2m1 is included too: it is the float's Ziv fallback. */

#define _GNU_SOURCE
#include <math.h>

#define exp2m1f(x) metallic_exp2m1f(x)
#include "src/math/float/exp2m1f.c"
#undef exp2m1f

#include "src/math/double/exp2m1.c"

#include <binary32/exp2m1/exp2m1f.c>   /* cr_exp2m1f */

#include "../../coremath.h"

static float fut(float x) { return metallic_exp2m1f(x); }
static float cr(float x) { return cr_exp2m1f(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

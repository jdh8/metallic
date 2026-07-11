/* Exhaustive bit-for-bit cross-check of Metallic exp10m1f against CORE-MATH's
 * correctly-rounded cr_exp10m1f over all 2^32 binary32 inputs.  The double
 * exp10m1 is included too: it is the float's Ziv fallback. */

#define _GNU_SOURCE
#include <math.h>

#define exp10m1f(x) metallic_exp10m1f(x)
#include "src/math/float/exp10m1f.c"
#undef exp10m1f

#include "src/math/double/exp10m1.c"

#include <binary32/exp10m1/exp10m1f.c>   /* cr_exp10m1f */

#include "../../coremath.h"

static float fut(float x) { return metallic_exp10m1f(x); }
static float cr(float x) { return cr_exp10m1f(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

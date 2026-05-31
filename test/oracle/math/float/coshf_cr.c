/* Exhaustive bit-for-bit cross-check of Metallic coshf against CORE-MATH's
 * correctly-rounded cr_coshf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in coshf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define coshf(x) metallic_coshf(x)
#include "src/math/float/coshf.c"
#undef coshf

#include <binary32/cosh/coshf.c>   /* cr_coshf */

#include "../../coremath.h"

static float fut(float x) { return metallic_coshf(x); }
static float cr(float x) { return cr_coshf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

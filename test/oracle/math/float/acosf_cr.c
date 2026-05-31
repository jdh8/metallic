/* Exhaustive bit-for-bit cross-check of Metallic acosf against CORE-MATH's
 * correctly-rounded cr_acosf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in acosf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define acosf(x) metallic_acosf(x)
#include "src/math/float/acosf.c"
#undef acosf

#include <binary32/acos/acosf.c>   /* cr_acosf */

#include "../../coremath.h"

static float fut(float x) { return metallic_acosf(x); }
static float cr(float x) { return cr_acosf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

/* Exhaustive bit-for-bit cross-check of Metallic sinf against CORE-MATH's
 * correctly-rounded cr_sinf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in sinf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define sinf(x) metallic_sinf(x)
#include "src/math/float/sinf.c"
#undef sinf
#include "src/math/float/rem_pio2f.c"

#include <binary32/sin/sinf.c>   /* cr_sinf */

#include "../../coremath.h"

static float fut(float x) { return metallic_sinf(x); }
static float cr(float x) { return cr_sinf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

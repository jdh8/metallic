/* Exhaustive bit-for-bit cross-check of Metallic tanhf against CORE-MATH's
 * correctly-rounded cr_tanhf over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in tanhf.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define tanhf(x) metallic_tanhf(x)
#include "src/math/float/tanhf.c"
#undef tanhf

#include <binary32/tanh/tanhf.c>   /* cr_tanhf */

#include "../../coremath.h"

static float fut(float x) { return metallic_tanhf(x); }
static float cr(float x) { return cr_tanhf(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

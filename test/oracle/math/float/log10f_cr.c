/* Exhaustive bit-for-bit cross-check of Metallic log10f against CORE-MATH's
 * correctly-rounded cr_log10f over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in log10f.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define log10f(x) metallic_log10f(x)
#include "src/math/float/log10f.c"
#undef log10f

#include <binary32/log10/log10f.c>   /* cr_log10f */

#include "../../coremath.h"

static float fut(float x) { return metallic_log10f(x); }
static float cr(float x) { return cr_log10f(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}

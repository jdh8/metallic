/* Exhaustive bit-for-bit cross-check of Metallic log2f against CORE-MATH's
 * correctly-rounded cr_log2f over all 2^32 binary32 inputs.  Both claim correct
 * rounding, so they must agree everywhere.  This is the fast gate; the MPFR
 * sweep in log2f.c is the slower independent ground-truth audit. */

#define _GNU_SOURCE
#include <math.h>

#define log2f(x) metallic_log2f(x)
#include "src/math/float/log2f.c"
#undef log2f

#include <binary32/log2/log2f.c>   /* cr_log2f */

#include "../../coremath.h"

static float fut(float x) { return metallic_log2f(x); }
static float cr(float x) { return cr_log2f(x); }

int main(void)
{
    return coremath_f32(fut, cr);
}
